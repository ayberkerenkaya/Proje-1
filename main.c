#include <stdio.h>
#include <time.h>

// Ders ba�lang�� ve biti� saatlerini saklamak i�in struct tan�m�
struct DersZamani {
    int yil;     // Y�l bilgisi (YYYY format�nda)
    int ay;      // Ay bilgisi (1-12 aras�nda)
    int gun;     // G�n bilgisi (1-31 aras�nda)
    int saat;    // Saat bilgisi (0-23 aras�nda)
    int dakika;  // Dakika bilgisi (0-59 aras�nda)
    int saniye;  // Saniye bilgisi (0-59 aras�nda)
};

// struct ve union birlikte kullan�l�yor
// Union, ayn� haf�zada hem struct hem de epoch zaman�n� tutmam�z� sa�lar
union EpochDonusturucu {
    struct DersZamani dz;  // Kullan�c�n�n girdi�i tarih ve saat bilgileri
    time_t epoch;          // Epoch zaman�na �evrilen de�er (saniye cinsinden)
};

// Kullan�c�dan ders zaman�n� al�p structa aktaran fonksiyon
void zamanAl(struct DersZamani *dz, const char *mesaj) {
    int gecersiz; // Kullan�c�n�n yanl�� veri girip girmedi�ini kontrol eden de�i�ken
    do {
        gecersiz = 0; // Varsay�lan olarak ge�erli kabul et
        printf("%s (YYYY MM DD HH MM SS): ", mesaj);
        scanf("%d %d %d %d %d %d", &dz->yil, &dz->ay, &dz->gun, &dz->saat, &dz->dakika, &dz->saniye);
        
        // Girilen de�erlerin ge�erlili�ini kontrol et
        if (dz->ay < 1 || dz->ay > 12) {
            printf("Gecersiz ay! 1-12 arasinda bir deger giriniz.\n");
            gecersiz = 1; // Yanl�� giri� yap�ld�, tekrar giri� istenecek
        }
        if (dz->gun < 1 || dz->gun > 31) {
            printf("Gecersiz gun! 1-31 arasinda bir deger giriniz.\n");
            gecersiz = 1;
        }
        if (dz->saat < 0 || dz->saat >= 24) {
            printf("Gecersiz saat! 0-23 arasinda bir deger giriniz.\n");
            gecersiz = 1;
        }
        if (dz->dakika < 0 || dz->dakika >= 60) {
            printf("Gecersiz dakika! 0-59 arasinda bir deger giriniz.\n");
            gecersiz = 1;
        }
        if (dz->saniye < 0 || dz->saniye >= 60) {
            printf("Gecersiz saniye! 0-59 arasinda bir deger giriniz.\n");
            gecersiz = 1;
        }
    } while (gecersiz); // Kullan�c� ge�ersiz giri� yaparsa tekrar giri� istenir
}

// Ders zaman�n� epoch zaman�na �eviren fonksiyon
time_t epochDonustur(struct DersZamani dz) {
    struct tm zamanBilgisi = {0}; // Zaman bilgisini struct i�ine at�yoruz
    
    zamanBilgisi.tm_year = dz.yil - 1900; // 1900'dan itibaren ge�en y�llar� hesapla
    zamanBilgisi.tm_mon = dz.ay - 1;      // Ay 0-11 aral���nda olmal�, bu y�zden 1 ��kar�yoruz
    zamanBilgisi.tm_mday = dz.gun;        // G�n oldu�u gibi atan�r
    zamanBilgisi.tm_hour = dz.saat;       // Saat bilgisi atan�r
    zamanBilgisi.tm_min = dz.dakika;      // Dakika bilgisi atan�r
    zamanBilgisi.tm_sec = dz.saniye;      // Saniye bilgisi atan�r
    
    return mktime(&zamanBilgisi); // struct bilgilerini epoch zaman�na �evir
}

int main() {
    union EpochDonusturucu baslangic, bitis; // Ba�lang�� ve biti� zamanlar�n� tutacak union de�i�kenleri
    
    // Kullan�c�dan ba�lang�� ve biti� zamanlar�n� al
    printf("Ders baslangic zamanini giriniz:\n");
    zamanAl(&baslangic.dz, "Ders baslangic zamani"); // Kullan�c�dan ba�lang�� zaman� al
    printf("Ders bitis zamanini giriniz:\n");
    zamanAl(&bitis.dz, "Ders bitis zamani"); // Kullan�c�dan biti� zaman� al
    
    // Al�nan zamanlar� epoch zaman�na �evir
    baslangic.epoch = epochDonustur(baslangic.dz);
    bitis.epoch = epochDonustur(bitis.dz);
    
    // Epoch d�n���m� ba�ar�s�z olursa hata mesaj� g�ster
    if (baslangic.epoch == -1 || bitis.epoch == -1) {
        printf("Epoch hesaplanirken hata olustu!\n");
        return 1; // Program� hata koduyla sonland�r
    }
    
    // Elde edilen epoch zamanlar�n� ekrana yazd�r
    printf("\nEpoch zamanlari:\n");
    printf("Ders baslangic zamani: %ld\n", baslangic.epoch);
    printf("Ders bitis zamani: %ld\n", bitis.epoch);
    
    // Ba�lang�� ve biti� aras�ndaki zaman fark�n� hesapla
    time_t fark = difftime(bitis.epoch, baslangic.epoch);
    printf("Ders suresi: %ld saniye (%ld dakika)\n", fark, fark / 60);
    
    return 0; // Program ba�ar�yla tamamland�
}

