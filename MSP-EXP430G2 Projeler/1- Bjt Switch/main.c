#include <msp430.h>

#define BUTTON BIT3  // P1.3 Buton pinini tanımlıyoruz
#define BJT_CONTROL BIT4  // P1.4 pinini BJT'yi tetiklemek için kullanacağız
#define LED1 BIT0   // P1.0 pinindeki LED (Butona basıldığında yanacak)
#define LED2 BIT6   // P1.6 pinindeki LED (Röle açıldığında yanacak)

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;  // Watchdog Timer'ı durduruyoruz

    // Port 1 ayarları
    P1DIR &= ~BUTTON;   // P1.3 buton pinini giriş olarak ayarlıyoruz
    P1REN |= BUTTON;    // P1.3 için pull-up/down direnç aktif ediyoruz
    P1OUT |= BUTTON;    // P1.3 pininde pull-up direnci aktif ediyoruz

    P1DIR |= BJT_CONTROL;  // P1.4 pinini çıkış olarak ayarlıyoruz (BJT tetikleme pin)
    P1OUT &= ~BJT_CONTROL; // Başlangıçta BJT'yi kapalı tutuyoruz

    P1DIR |= LED1;  // P1.0 pinini çıkış olarak ayarlıyoruz (LED1 çıkışı)
    P1OUT &= ~LED1; // Başlangıçta LED1'i kapalı tutuyoruz

    P1DIR &= ~LED2;  // P1.6 pinini giriş olarak ayarlıyoruz (LED2 giriş olarak)
    P1REN |= LED2;   // P1.6 için pull-up/down direnç ekliyoruz
    P1OUT &= ~LED2;  // Başlangıçta LED2'yi kapalı tutuyoruz

    while(1) {
        // Butona basıldığını kontrol ediyoruz
        if ((P1IN & BUTTON) == 0) {  // Butona basıldıysa
            __delay_cycles(100000);  // Debounce için kısa bir bekleme

            // P1.0'daki LED'i yakıyoruz (Butona basıldığında)
            P1OUT |= LED1;

            // BJT'yi tetiklemek için P1.4'ü HIGH yapıyoruz
            P1OUT |= BJT_CONTROL;  // BJT'yi tetikleyerek röleyi açıyoruz
        } else {
            // Butona basılmadığında, BJT'yi kapalı tutuyoruz
            P1OUT &= ~BJT_CONTROL;  // BJT'yi kapatıyoruz
            P1OUT &= ~LED1;  // LED1'i kapatıyoruz (Butona basılmadı)
        }

        // Röle durumu kontrol ediliyor
        if (P1OUT & BJT_CONTROL) {  // BJT tetiklendiğinde (röle açık)
            // Röle açık olduğunda, P1.6 pinine HIGH sinyali veriyoruz (LED2'yi yakıyoruz)
            P1OUT |= LED2;
        } else {
            // BJT tetiklenmediğinde (röle kapalı), P1.6 pinini LOW yapıyoruz (LED2'yi kapatıyoruz)
            P1OUT &= ~LED2;
        }
    }
}
