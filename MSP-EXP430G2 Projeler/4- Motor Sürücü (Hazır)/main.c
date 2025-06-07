#include <msp430.h>

volatile unsigned char mode = 0; // 0: %25, 1: %75, 2: %100
volatile unsigned char system_on = 0;

void setupPWM() {
    P1DIR |= BIT2;         // P1.2 çıkış olarak ayarlandı
    P1SEL |= BIT2;         // P1.2, Timer_A çıkışı olarak ayarlandı
    TA0CCR0 = 1000;        // PWM periyodu
    TA0CCR1 = 250;         // Başlangıçta %25 duty cycle
    TA0CCTL1 = OUTMOD_7;   // Reset/Set modu
    TA0CTL = TASSEL_2 + MC_1; // SMCLK, yukarı sayma modu
}

void updatePWM() {
    switch (mode) {
        case 0:
            TA0CCR1 = 250;  // %25 duty cycle
            break;
        case 1:
            TA0CCR1 = 750;  // %75 duty cycle
            break;
        case 2:
            TA0CCR1 = 1000; // %100 duty cycle
            break;
    }
}

void main() {
    WDTCTL = WDTPW | WDTHOLD; // Watchdog kapalı
    P1DIR |= BIT0 + BIT6;    // P1.0 ve P1.6 çıkış (LED)
    P1OUT &= ~(BIT0 + BIT6); // LED'ler kapalı

    P1DIR &= ~(BIT3 + BIT4); // P1.3 ve P1.4 giriş (Butonlar)
    P1REN |= BIT3 + BIT4;    // Pull-up/down direnci aktif
    P1OUT |= BIT3 + BIT4;    // Pull-up direnci

    P1IE |= BIT3 + BIT4;     // P1.3 ve P1.4 kesme aktif
    P1IFG &= ~(BIT3 + BIT4); // Kesme bayrakları temizlendi

    setupPWM();

    __bis_SR_register(GIE);  // Global kesme aktif

    while (1) {
        if (system_on) {
            P1OUT &= ~BIT0; // Sistem açık, LED kapalı
        } else {
            P1OUT |= BIT0;  // Sistem kapalı, LED açık
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & BIT3) { // ON/OFF butonu
        system_on = !system_on;
        if (system_on) {
            P1OUT |= BIT6;  // Mod LED'i anlık yanıp söner
            __delay_cycles(100000);
            P1OUT &= ~BIT6;
        }
        P1IFG &= ~BIT3; // Bayrağı temizle
    }

    if (P1IFG & BIT4) { // Mod değişiklik butonu
        if (system_on) {
            mode = (mode + 1) % 3;
            updatePWM();
            P1OUT |= BIT6;  // Mod LED'i anlık yanıp söner
            __delay_cycles(100000);
            P1OUT &= ~BIT6;
        }
        P1IFG &= ~BIT4; // Bayrağı temizle
    }
}
