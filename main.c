#include <stdio.h>
#include <time.h>

// Ders baþlangýç ve bitiþ saatlerini saklamak için struct tanýmý
struct DersZamani {
    int yil;     // Yýl bilgisi (YYYY formatýnda)
    int ay;      // Ay bilgisi (1-12 arasýnda)
    int gun;     // Gün bilgisi (1-31 arasýnda)
    int saat;    // Saat bilgisi (0-23 arasýnda)
    int dakika;  // Dakika bilgisi (0-59 arasýnda)
    int saniye;  // Saniye bilgisi (0-59 arasýnda)
};

// struct ve union birlikte kullanýlýyor
// Union, ayný hafýzada hem struct hem de epoch zamanýný tutmamýzý saðlar
union EpochDonusturucu {
    struct DersZamani dz;  // Kullanýcýnýn girdiði tarih ve saat bilgileri
    time_t epoch;          // Epoch zamanýna çevrilen deðer (saniye cinsinden)
};

// Kullanýcýdan ders zamanýný alýp structa aktaran fonksiyon
void zamanAl(struct DersZamani *dz, const char *mesaj) {
    int gecersiz; // Kullanýcýnýn yanlýþ veri girip girmediðini kontrol eden deðiþken
    do {
        gecersiz = 0; // Varsayýlan olarak geçerli kabul et
        printf("%s (YYYY MM DD HH MM SS): ", mesaj);
        scanf("%d %d %d %d %d %d", &dz->yil, &dz->ay, &dz->gun, &dz->saat, &dz->dakika, &dz->saniye);
        
        // Girilen deðerlerin geçerliliðini kontrol et
        if (dz->ay < 1 || dz->ay > 12) {
            printf("Gecersiz ay! 1-12 arasinda bir deger giriniz.\n");
            gecersiz = 1; // Yanlýþ giriþ yapýldý, tekrar giriþ istenecek
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
    } while (gecersiz); // Kullanýcý geçersiz giriþ yaparsa tekrar giriþ istenir
}

// Ders zamanýný epoch zamanýna çeviren fonksiyon
time_t epochDonustur(struct DersZamani dz) {
    struct tm zamanBilgisi = {0}; // Zaman bilgisini struct içine atýyoruz
    
    zamanBilgisi.tm_year = dz.yil - 1900; // 1900'dan itibaren geçen yýllarý hesapla
    zamanBilgisi.tm_mon = dz.ay - 1;      // Ay 0-11 aralýðýnda olmalý, bu yüzden 1 çýkarýyoruz
    zamanBilgisi.tm_mday = dz.gun;        // Gün olduðu gibi atanýr
    zamanBilgisi.tm_hour = dz.saat;       // Saat bilgisi atanýr
    zamanBilgisi.tm_min = dz.dakika;      // Dakika bilgisi atanýr
    zamanBilgisi.tm_sec = dz.saniye;      // Saniye bilgisi atanýr
    
    return mktime(&zamanBilgisi); // struct bilgilerini epoch zamanýna çevir
}

int main() {
    union EpochDonusturucu baslangic, bitis; // Baþlangýç ve bitiþ zamanlarýný tutacak union deðiþkenleri
    
    // Kullanýcýdan baþlangýç ve bitiþ zamanlarýný al
    printf("Ders baslangic zamanini giriniz:\n");
    zamanAl(&baslangic.dz, "Ders baslangic zamani"); // Kullanýcýdan baþlangýç zamaný al
    printf("Ders bitis zamanini giriniz:\n");
    zamanAl(&bitis.dz, "Ders bitis zamani"); // Kullanýcýdan bitiþ zamaný al
    
    // Alýnan zamanlarý epoch zamanýna çevir
    baslangic.epoch = epochDonustur(baslangic.dz);
    bitis.epoch = epochDonustur(bitis.dz);
    
    // Epoch dönüþümü baþarýsýz olursa hata mesajý göster
    if (baslangic.epoch == -1 || bitis.epoch == -1) {
        printf("Epoch hesaplanirken hata olustu!\n");
        return 1; // Programý hata koduyla sonlandýr
    }
    
    // Elde edilen epoch zamanlarýný ekrana yazdýr
    printf("\nEpoch zamanlari:\n");
    printf("Ders baslangic zamani: %ld\n", baslangic.epoch);
    printf("Ders bitis zamani: %ld\n", bitis.epoch);
    
    // Baþlangýç ve bitiþ arasýndaki zaman farkýný hesapla
    time_t fark = difftime(bitis.epoch, baslangic.epoch);
    printf("Ders suresi: %ld saniye (%ld dakika)\n", fark, fark / 60);
    
    return 0; // Program baþarýyla tamamlandý
}

