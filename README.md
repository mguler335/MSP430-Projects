# 📟 MSP430 Microcontroller Projects

Bu depo, Texas Instruments **MSP430G2553** (Launchpad) mikrodenetleyicisi kullanılarak geliştirilmiş çeşitli temel ve ileri seviye gömülü sistem uygulamalarını içermektedir. Projeler, mikrodenetleyici mimarisini anlamak ve çevre birimlerini (peripherals) kontrol etmek amacıyla hazırlanmıştır.



## 🚀 Proje İçerikleri

### 💡 Temel I/O ve Kesme İşlemleri
Mikrodenetleyicinin giriş-çıkış pinlerini ve donanımsal kesme (Interrupt) mekanizmalarını kapsar.
* **Led Blinking:** Dahili LED'lerin belirli zaman aralıklarıyla yakılıp söndürülmesi.
* **Button Control:** Buton girişi ile LED durumunun kontrol edilmesi ve Polling/Interrupt yöntemlerinin farkları.
* **Software Debouncing:** Buton arkı (bouncing) sorununu gidermek için yazılımsal gecikme algoritmaları.

### ⏱️ Zamanlayıcı ve PWM Uygulamaları
Timer birimlerini kullanarak hassas zamanlama ve sinyal üretimi sağlar.
* **Timer Interrupts:** İşlemciyi meşgul etmeden (non-blocking) belirli periyotlarda görevlerin icra edilmesi.
* **LED Fading (PWM):** Donanımsal PWM kullanarak bir LED'in parlaklığının yumuşak bir şekilde artırılıp azaltılması.
* **Morse Code:** Timer kesmeleri kullanarak girilen metnin Mors alfabesiyle LED üzerinden sinyalleştirilmesi.

### 📡 Haberleşme ve Sensör Entegrasyonu
Dış dünya ve diğer cihazlarla veri alışverişi sağlayan protokol uygulamalarıdır.
* **UART Echo:** Bilgisayar ile seri haberleşme kurarak gelen verilerin geri gönderilmesi.
* **ADC Test:** Dahili sıcaklık sensöründen veya potansiyometreden gelen analog sinyallerin dijital veriye dönüştürülmesi.
* **Timer Capture:** Dışarıdan gelen bir sinyalin frekansını veya periyodunu donanımsal yakalama (Capture) modu ile ölçme.

## 🛠️ Teknik Detaylar
* **Mikrodenetleyici:** MSP430G2553
* **Geliştirme Ortamı:** Code Composer Studio (CCS)
* **Programlama Dili:** C
* **Donanım:** MSP-EXP430G2 LaunchPad

## 🔌 Kurulum ve Kullanım
1. **IDE Kurulumu:** Texas Instruments'ın sağladığı Code Composer Studio'yu yükleyin.
2. **Proje Aktarımı:** İlgili proje klasörlerini Workspace'inize import edin.
3. **Derleme:** Projeyi derleyin ve USB üzerinden bağlı Launchpad kartınıza yükleyin.
4. **Haberleşme:** UART projeleri için 9600 Baudrate değerinde bir seri terminal (Putty, Tera Term vb.) kullanın.

---
*Bu çalışma Muhammed Halil Güler tarafından gömülü sistemler ve mikrodenetleyici mimarisi eğitimi kapsamında geliştirilmiştir.*
