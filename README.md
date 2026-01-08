# 📟 MSP430 Microcontroller Projects
![WhatsApp Görsel 2025-04-23 saat 21 01 10_a863c54c](https://github.com/user-attachments/assets/26a7a568-9ef1-4adc-a644-200ba72b88ec)

Bu depo, Texas Instruments **MSP430G2553** (Launchpad) mikrodenetleyicisi kullanılarak geliştirilmiş çeşitli temel ve ileri seviye gömülü sistem uygulamalarını içermektedir. Projeler, mikrodenetleyici mimarisini anlamak ve çevre birimlerini (peripherals) kontrol etmek amacıyla hazırlanmıştır.



# 📟 MSP430 Microcontroller & Industrial CANBus Projects

Bu depo, Texas Instruments **MSP430G2553** mikrodenetleyicisi kullanılarak geliştirilmiş temel gömülü sistem uygulamalarının yanı sıra, **Nurol Makina** bünyesindeki ortak eğitim sürecinde gerçekleştirilen endüstriyel haberleşme (CANBus) projelerini içermektedir.



## 🏗️ Öne Çıkan Proje: Endüstriyel CANBus Haberleşmesi
Bu çalışma, askeri araç elektroniği standartlarında kullanılan CANBus protokolünün MSP430 üzerinde gerçeklenmesini kapsar.

* **Donanım:** MSP430G2553 ve SPI arayüzü üzerinden haberleşen **MCP2515 CAN Controller**.
* **Haberleşme Ağı:** SPC560 (PeakCAN) sisteminden gelen CAN mesajlarının algılanması, ID tabanlı filtrelenmesi ve yanıtlanması.
* **Protokol Detayları:** * SPI üzerinden MCP2515 konfigürasyonu.
    * Standart ve Genişletilmiş (Extended) ID takibi.
    * Farklı baudrate değerlerinde (250kbps/500kbps) kararlı veri iletimi.
* **Uygulama:** Araç üzerindeki farklı kontrol ünitelerinin (ECU) simülasyonu ve mesaj trafik analizi.

## 🚀 Diğer Uygulamalar

### ⏱️ Zamanlayıcı ve Kesme Yönetimi
* **Non-Blocking Programlama:** `__bis_SR_register(LPM0_bits + GIE)` kullanarak düşük güç tüketimi ve kesme tabanlı (Interrupt) mimari.
* **PWM Sinyal Üretimi:** Timer_A birimi ile motor sürücü veya LED parlaklık kontrolü.
* **Debouncing:** Buton arklarını önleyen yazılımsal filtreleme algoritmaları.

### 📡 Çevre Birimleri (Peripherals)
* **UART:** Bilgisayar arayüzü ile veri terminali üzerinden haberleşme (Baudrate senkronizasyonu).
* **ADC10:** Dahili veya harici analog verilerin 10-bit çözünürlükle işlenmesi.
* **SPI/I2C:** Sensör ve haberleşme modülleriyle (MCP2515 vb.) senkron veri aktarımı.

## 🛠️ Teknik Bilgiler
* **Mikrodenetleyici:** TI MSP430G2553 (Ultra-Low Power)
* **Geliştirme Ortamı:** Code Composer Studio (CCS)
* **Dil:** Embedded C
* **Donanım:** MSP-EXP430G2 LaunchPad & MCP2515 CAN Board

## 🔌 Kullanım
1. Projeyi CCS (Code Composer Studio) içerisine import edin.
2. `main.c` dosyasındaki saat konfigürasyonunun (DCO) kartınıza uygun olduğundan emin olun.
3. CANBus projeleri için MSP430 ve MCP2515 arasındaki SPI (MISO, MOSI, CLK, CS) bağlantılarını kontrol edin.

---
