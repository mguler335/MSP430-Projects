#include <msp430.h>

// MCP2515 SPI komutları
#define MCP_RESET       0xC0
#define MCP_READ        0x03
#define MCP_WRITE       0x02
#define MCP_RTS_TXB0    0x81
#define MCP_READ_STATUS 0xA0

// MCP2515 Register Adresleri
#define CANCTRL         0x0F
#define CNF1            0x2A
#define CNF2            0x29
#define CNF3            0x28
#define TXB0SIDH        0x31
#define TXB0SIDL        0x32
#define TXB0DLC         0x35
#define TXB0D0          0x36
#define TXB0CTRL        0x30

#define SPI_CS_LOW      (P2OUT &= ~BIT0)  // CS pinini LOW çek
#define SPI_CS_HIGH     (P2OUT |= BIT0)   // CS pinini HIGH çek

void SPI_init(void);
unsigned char SPI_transfer(unsigned char data);
void MCP2515_reset(void);
void MCP2515_writeRegister(unsigned char address, unsigned char value);
unsigned char MCP2515_readRegister(unsigned char address);
void MCP2515_init(void);
void MCP2515_sendCANMessage(void);

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Watchdog Timer'ı durdur

    // SPI ve MCP2515 başlat
    SPI_init();
    MCP2515_reset();
    MCP2515_init();

    while (1) {
        MCP2515_sendCANMessage();  // CAN mesajı gönder
        __delay_cycles(102090);   // Yaklaşık 1 saniye bekle
    }
}

// SPI Modülünü Başlat
void SPI_init(void) {
    P1SEL |= BIT5 + BIT6 + BIT7;  // SCK, MISO, MOSI pinlerini SPI olarak ayarla
    P1SEL2 |= BIT5 + BIT6 + BIT7;
    P2DIR |= BIT0;   // P2.0 (CS pini) çıkış olarak ayarla
    SPI_CS_HIGH;     // MCP2515 CS'yi HIGH yap

    UCB0CTL1 |= UCSWRST;  // SPI modülünü durdur
    UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC;  // SPI master, senkron, MSB first
    UCB0CTL1 = UCSSEL_2;  // SMCLK kullan
    UCB0BR0 = 0x04;  // SPI Clock Divider (SMCLK için uygun değer)
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;  // SPI modülünü başlat
}

// SPI ile veri gönderme/alma
unsigned char SPI_transfer(unsigned char data) {
    while (!(IFG2 & UCB0TXIFG));  // TX buffer boş mu?
    UCB0TXBUF = data;  // Veri gönder
    while (!(IFG2 & UCB0RXIFG));  // RX buffer doldu mu?
    return UCB0RXBUF;  // Gelen veriyi oku
}

// MCP2515 RESET komutunu gönder
void MCP2515_reset(void) {
    SPI_CS_LOW;
    SPI_transfer(MCP_RESET);
    SPI_CS_HIGH;
    __delay_cycles(200000);  // MCP2515'in yeniden başlaması için yeterli süre
}

// MCP2515 register yazma
void MCP2515_writeRegister(unsigned char address, unsigned char value) {
    SPI_CS_LOW;
    SPI_transfer(MCP_WRITE);
    SPI_transfer(address);
    SPI_transfer(value);
    SPI_CS_HIGH;
}

// MCP2515 register okuma
unsigned char MCP2515_readRegister(unsigned char address) {
    unsigned char value;
    SPI_CS_LOW;
    SPI_transfer(MCP_READ);
    SPI_transfer(address);
    value = SPI_transfer(0x00);
    SPI_CS_HIGH;
    return value;
}

// MCP2515'i 500kbps CAN hızı ile başlat
void MCP2515_init(void) {
    MCP2515_writeRegister(CANCTRL, 0x80);  // Konfigürasyon modu

    // 500 kbps ayarları (8MHz kristal için)
    MCP2515_writeRegister(CNF1, 0x00);
    MCP2515_writeRegister(CNF2, 0x90);
    MCP2515_writeRegister(CNF3, 0x02);

    MCP2515_writeRegister(CANCTRL, 0x00);  // Normal moda al
    // Çalışma modunun kontrolü (isteğe bağlı)
    if ((MCP2515_readRegister(CANCTRL) & 0xE0) != 0x00) {
        // Hata: Normal moda geçilemedi
        while (1);  // Burada hata işleme yapılabilir
    }
}

// CAN mesajı gönderme fonksiyonu
void MCP2515_sendCANMessage(void) {
    // 11-bit ID: 0x100
    MCP2515_writeRegister(TXB0SIDH, 0x04);  
    MCP2515_writeRegister(TXB0SIDL, 0x00);

    // Veri uzunluğu: 8 byte
    MCP2515_writeRegister(TXB0DLC, 0x08);

    // Veri yükü (örnek mesaj)
    MCP2515_writeRegister(TXB0D0, 0x11);
    MCP2515_writeRegister(TXB0D0 + 1, 0x22);
    MCP2515_writeRegister(TXB0D0 + 2, 0x33);
    MCP2515_writeRegister(TXB0D0 + 3, 0x44);
    MCP2515_writeRegister(TXB0D0 + 4, 0x55);
    MCP2515_writeRegister(TXB0D0 + 5, 0x66);
    MCP2515_writeRegister(TXB0D0 + 6, 0x77);
    MCP2515_writeRegister(TXB0D0 + 7, 0x88);

    // Gönderme kontrolü
    SPI_CS_LOW;
    SPI_transfer(MCP_RTS_TXB0);
    SPI_CS_HIGH;

    // Mesaj gönderildi mi kontrolü
    if (MCP2515_readRegister(TXB0CTRL) & 0x08) {
        // Mesaj başarıyla gönderildi
    } else {
        // Gönderim hatası
    }
}
