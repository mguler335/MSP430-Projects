#include <msp430.h>

volatile unsigned int duty_cycle = 25;   // PWM duty cycle (başlangıç %25)
volatile unsigned char system_on = 0;   // Sistem durumu (0: OFF, 1: ON)

// Timer ve GPIO ayarları
void setup_pwm() {
    // P1.2 PWM çıkışı olarak ayarla
    P1DIR |= BIT2;    // P1.2 çıkış olarak ayarlandı
    P1SEL |= BIT2;    // P1.2 Timer_A'nın çıkışı olarak ayarlandı

    // Timer_A ayarları
    TA0CCR0 = 1000 - 1;        // PWM frekansı (~1 kHz, SMCLK 1 MHz ise)
    TA0CCTL1 = OUTMOD_7;       // CCR1 reset/set modu
    TA0CCR1 = (TA0CCR0 * duty_cycle) / 100; // Duty cycle ayarla
    TA0CTL = TASSEL_2 + MC_1;  // SMCLK, up mode
}

// GPIO ve buton kesme ayarları
void setup_gpio() {
    // P1.0 ve P1.6 LED'ler için çıkış ayarları
    P1DIR |= BIT0 + BIT6;      // P1.0 ve P1.6 çıkış
    P1OUT &= ~(BIT0 + BIT6);   // LED'ler kapalı

    // P2.4 ve P1.3 butonlar için giriş ayarları
    P2DIR &= ~BIT4;            // P2.4 giriş
    P2REN |= BIT4;             // Pull-up/down direnci etkin
    P2OUT |= BIT4;             // Pull-up direnci (buton normalde HIGH)

    P1DIR &= ~BIT3;            // P1.3 giriş
    P1REN |= BIT3;             // Pull-up/down direnci etkin
    P1OUT |= BIT3;             // Pull-up direnci (buton normalde HIGH)

    // Buton kesme ayarları
    P2IES |= BIT4;             // P2.4 kenar tetikleme (HIGH-to-LOW)
    P2IFG &= ~BIT4;            // Kesme bayrağını temizle
    P2IE |= BIT4;              // P2.4 kesmesini etkinleştir

    P1IES |= BIT3;             // P1.3 kenar tetikleme (HIGH-to-LOW)
    P1IFG &= ~BIT3;            // Kesme bayrağını temizle
    P1IE |= BIT3;              // P1.3 kesmesini etkinleştir
}

// P2.4 buton kesmesi
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
    if (P2IFG & BIT4) {
        P2IFG &= ~BIT4;        // Kesme bayrağını temizle

        if (system_on) {
            // PWM duty cycle değerini sırayla değiştir
            if (duty_cycle == 25) {
                duty_cycle = 75;
            } else if (duty_cycle == 75) {
                duty_cycle = 100;
            } else {
                duty_cycle = 25;
            }
            TA0CCR1 = (TA0CCR0 * duty_cycle) / 100; // Yeni duty cycle uygula

            // P1.6 LED'i kısa süre yakıp söndür
            P1OUT |= BIT6;
            __delay_cycles(100000); // Kısa bir gecikme (~100ms)
            P1OUT &= ~BIT6;
        }
    }
}

// P1.3 buton kesmesi
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & BIT3) {
        P1IFG &= ~BIT3;        // Kesme bayrağını temizle

        system_on = !system_on; // Sistem durumunu değiştir

        if (system_on) {
            P1OUT &= ~BIT0;    // Sistem ON, P1.0 LED'i kapalı
        } else {
            P1OUT |= BIT0;     // Sistem OFF, P1.0 LED'i açık
            TA0CCR1 = 0;       // PWM çıkışını durdur
        }
    }
}

// Ana fonksiyon
int main(void) {
    WDTCTL = WDTPW + WDTHOLD;  // Watchdog Timer'ı durdur
    BCSCTL1 = CALBC1_1MHZ;     // SMCLK 1 MHz
    DCOCTL = CALDCO_1MHZ;

    setup_gpio();              // GPIO ve buton ayarları
    setup_pwm();               // PWM ayarları

    __enable_interrupt();      // Kesmelere izin ver

    while (1) {
        __low_power_mode_0();  // LPM0 moduna gir
    }
}
