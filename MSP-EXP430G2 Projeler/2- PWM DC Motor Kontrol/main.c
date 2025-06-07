#include <msp430.h>

#define BUTTON_PRESSED (P1IN & BIT3)  // Butonun basılı olup olmadığını kontrol et
#define DEBOUNCE_DELAY 20000          // Debounce gecikmesi (butonun düzgün algılanması için)

void main(void)
{
    // Temel ayarlamalar
    WDTCTL = WDTPW | WDTHOLD;   // Watchdog timer'ı kapat
    BCSCTL1 = CALBC1_1MHZ;       // 1 MHz saat kaynağını kullan
    DCOCTL = CALDCO_1MHZ;

    // P1.2 pinini çıkış olarak ayarla (PWM çıkışı)
    P1DIR |= BIT2;               // P1.2 çıkış
    P1SEL |= BIT2;               // P1.2'yi Timer A0.1 çıkışı olarak seç

    // P1.0 ve P1.6 pinlerini çıkış olarak ayarla (LED'ler)
    P1DIR |= BIT0;               // P1.0 çıkış (LED)
    P1DIR |= BIT6;               // P1.6 çıkış (LED)

    // P1.3 pinini giriş olarak ayarla (Buton)
    P1DIR &= ~BIT3;              // P1.3 giriş
    P1REN |= BIT3;               // P1.3 için pull-up/pull-down direnç aktif
    P1OUT |= BIT3;               // Pull-up direnci

    // Timer A ayarları
    TACCTL1 = OUTMOD_7;          // PWM çıkışı (RESET/SET modunda)
    TACTL = TASSEL_2 + MC_1;     // Timer A'yı SMCLK ile çalıştır, yükselen kenarda sayacı başlat
    TACCR0 = 5000;               // Timer döngü süresi (yaklaşık 5ms periyot)

    // Başlangıçta motor %75 güçle çalışacak
    unsigned int pwm_duty = 3750; // %75 PWM genişliği (3750/5000)
    TACCR1 = pwm_duty;           // PWM genişliği (ilk modda %75 güç)

    unsigned char mode = 0;      // Başlangıç modunu 0 (motor %75 güçte çalışacak) olarak ayarla

    // Sonsuz döngü
    while(1)
    {
        // Butonun durumunu kontrol et
        if (!(BUTTON_PRESSED))  // Buton basıldığında mod değişecek
        {
            __delay_cycles(DEBOUNCE_DELAY);  // Butonun bounces etkisini engellemek için kısa bir gecikme
            if (!(BUTTON_PRESSED)) {        // Buton gerçekten basıldığında geçişi gerçekleştir
                mode = !mode;              // Modu değiştir
                if (mode == 0) {
                    pwm_duty = 3750;    // %75 PWM genişliği
                    TACCR1 = pwm_duty;  // PWM darbe genişliği (75% duty cycle)
                    P1OUT |= BIT0;       // P1.0 LED yanacak (ilk mod)
                    P1OUT &= ~BIT6;      // P1.6 LED sönük
                } else {
                    pwm_duty = 5000;    // %100 PWM genişliği
                    TACCR1 = pwm_duty;  // PWM darbe genişliği (100% duty cycle)
                    P1OUT &= ~BIT0;      // P1.0 LED sönük
                    P1OUT |= BIT6;       // P1.6 LED yanacak (ikinci mod)
                }
            }
        }
    }
}
