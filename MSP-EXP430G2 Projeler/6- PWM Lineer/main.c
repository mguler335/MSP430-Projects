#include <msp430.h>

volatile unsigned int adc_value = 0;     // ADC'den okunan değer
volatile float temperature = 0;          // Hesaplanan sıcaklık
volatile unsigned int duty_cycle = 0;    // PWM duty cycle

// ADC ve sıcaklık sensörü ayarları
void setup_adc() {
    ADC10CTL1 = INCH_10;       // Sıcaklık sensörü kanalı (INCH_10)
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE; // Dahili referans ve ADC etkin
    __delay_cycles(1000);      // Referansın stabilize olması için gecikme
}

// Timer ve PWM ayarları
void setup_pwm() {
    P1DIR |= BIT2;             // P1.2 çıkış olarak ayarlandı (PWM çıkışı)
    P1SEL |= BIT2;             // P1.2 Timer_A çıkışı
    TA0CCR0 = 1000 - 1;        // PWM frekansı (~1kHz, SMCLK = 1MHz)
    TA0CCTL1 = OUTMOD_7;       // CCR1 reset/set modu
    TA0CCR1 = 0;               // Başlangıç duty cycle (0)
    TA0CTL = TASSEL_2 + MC_1;  // SMCLK, up mode
}

// GPIO ayarları
void setup_gpio() {
    P1DIR |= BIT0;             // P1.0 çıkış olarak ayarlandı
    P1OUT &= ~BIT0;            // P1.0 LED'i başlangıçta kapalı
}

// PWM duty cycle'ı güncelle
void update_pwm() {
    if (temperature <= 30) {
        duty_cycle = 0;            // Fan kapalı
        TA0CCR1 = 0;              // PWM duty cycle 0 (fan kapalı)
        P1OUT &= ~BIT0;           // LED kapalı
    } else if (temperature >= 85) {
        duty_cycle = 100;          // Fan tam hız
        TA0CCR1 = (TA0CCR0 * duty_cycle) / 100;    // PWM güncelle
        P1OUT |= BIT0;            // LED açık
    } else {
        duty_cycle = (unsigned int)((temperature - 30) * 100 / (85 - 30)); // Doğrusal PWM
        TA0CCR1 = (TA0CCR0 * duty_cycle) / 100;    // PWM güncelle
        P1OUT |= BIT0;            // LED açık
    }
}

// Sıcaklığı hesapla (ADC değerinden °C'ye çevir)
void calculate_temperature() {
    // Datasheet’teki kalibrasyon sabitlerini kullan
    float temp_30 = 673;  // 30°C'deki ADC değeri (~1.5V)
    float temp_85 = 923;  // 85°C'deki ADC değeri (~2.0V)
    temperature = ((adc_value - temp_30) * (85 - 30)) / (temp_85 - temp_30) + 30;
}

// ADC kesmesi
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
    adc_value = ADC10MEM;         // ADC değeri oku
    calculate_temperature();      // Sıcaklığı hesapla
    update_pwm();                 // PWM güncelle
    __bic_SR_register_on_exit(CPUOFF); // CPU'yu uyandır
}

// Ana fonksiyon
int main(void) {
    WDTCTL = WDTPW + WDTHOLD;    // Watchdog Timer'ı durdur
    BCSCTL1 = CALBC1_1MHZ;       // SMCLK 1 MHz
    DCOCTL = CALDCO_1MHZ;

    setup_adc();                 // ADC ayarları
    setup_pwm();                 // PWM ayarları
    setup_gpio();                // GPIO ayarları

    __enable_interrupt();        // Kesmelere izin ver

    while (1) {
        ADC10CTL0 |= ENC + ADC10SC;   // ADC ölçümünü başlat
        __bis_SR_register(CPUOFF + GIE); // ADC kesmesi tamamlanana kadar uyku moduna gir
        __delay_cycles(500000);      // 500ms sıcaklık ölçüm döngüsü
    }
}
