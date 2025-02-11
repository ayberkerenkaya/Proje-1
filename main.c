#include <stdio.h>
#include <time.h>

// Ders baslangic ve bitis saatlerini saklamak icin struct
struct DersZamani {
    int yil;
    int ay;
    int gun;
    int saat;
    int dakika;
    int saniye;
};

// struct ve union
union EpochDonusturucu {
    struct DersZamani dz;
    time_t epoch;
};

// Kullanicidan ders zamanini alma ve strucka atama
void zamanAl(struct DersZamani *dz, const char *mesaj) {
    int gecersiz;
    do {
        gecersiz = 0;
        printf("%s (YYYY MM DD HH MM SS): ", mesaj);
        scanf("%d %d %d %d %d %d", &dz->yil, &dz->ay, &dz->gun, &dz->saat, &dz->dakika, &dz->saniye);
        
        if (dz->ay < 1 || dz->ay > 12) {
            printf("Gecersiz ay! 1-12 arasinda bir deger giriniz.\n");
            gecersiz = 1;
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
    } while (gecersiz);
}

// ders zamanýný struct epoch zamanina cevirme
time_t epochDonustur(struct DersZamani dz) {
    struct tm zamanBilgisi = {0};
    zamanBilgisi.tm_year = dz.yil - 1900; // 1900 den itibaren kaç yýl geçtiðini hesapla
    zamanBilgisi.tm_mday = dz.gun;
    zamanBilgisi.tm_hour = dz.saat;
    zamanBilgisi.tm_min = dz.dakika;
    zamanBilgisi.tm_sec = dz.saniye;
    return mktime(&zamanBilgisi);
}

int main() {
    union EpochDonusturucu baslangic, bitis;
    
    printf("Ders baslangic zamanini giriniz:\n");
    zamanAl(&baslangic.dz, "Ders baslangic zamani");
    printf("Ders bitis zamanini giriniz:\n");
    zamanAl(&bitis.dz, "Ders bitis zamani");
    
    baslangic.epoch = epochDonustur(baslangic.dz);
    bitis.epoch = epochDonustur(bitis.dz);
    
    if (baslangic.epoch == -1 || bitis.epoch == -1) {
        printf("Epoch hesaplanirken hata olustu!\n");
        return 1;
    }
    
    printf("\nEpoch zamanlari:\n");
    printf("Ders baslangic zamani: %ld\n", baslangic.epoch);
    printf("Ders bitis zamani: %ld\n", bitis.epoch);
    
    time_t fark = difftime(bitis.epoch, baslangic.epoch);
    printf("Ders suresi: %ld saniye (%ld dakika)\n", fark, fark / 60);
    
    return 0;
}

