#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_YEMEK 100
#define MAX_SIPARIS 100

typedef struct {
    char yemekAdi[50];
    double fiyat;
    int hazirlamaSuresi;
    char durum[10];
} Yemek;

typedef struct {
    char siparisID[20];
    char yemekAdi[50];
    double fiyat;
    char siparisZamani[20];
    char hazirlamaZamani[20];
    char kullaniciAdi[20];
    char asci[5];
} Siparis;

// Fonksiyon Prototipleri
void yemekEkle();
void yemekGuncelle();
void yemekSil();
void yeniSiparisOnay();
void gunlukRapor();
void analizler();
void asciSayisiBelirle();

// Yemek ekleme
void yemekEkle() {
    FILE *file = fopen("yemeklistesi.txt", "a");
    if (file == NULL) {
        printf("yemeklistesi.txt dosyasi acilamadi.\n");
        exit(1);
    }

    Yemek yeniYemek;
    printf("Yemek Adi: ");
    scanf("%s", yeniYemek.yemekAdi);
    printf("Fiyat: ");
    scanf("%lf", &yeniYemek.fiyat);
    printf("Hazirlama Suresi (dakika): ");
    scanf("%d", &yeniYemek.hazirlamaSuresi);
    strcpy(yeniYemek.durum, "Mevcut");

    fprintf(file, "%s %.2f %d %s\n", yeniYemek.yemekAdi, yeniYemek.fiyat, yeniYemek.hazirlamaSuresi, yeniYemek.durum);
    fclose(file);

    printf("Yemek basariyla eklendi.\n");
}

// Yemek güncelleme
void yemekGuncelle() {
    FILE *file = fopen("yemeklistesi.txt", "r");
    if (file == NULL) {
        printf("yemeklistesi.txt dosyasi acilamadi.\n");
        exit(1);
    }

    Yemek yemekler[MAX_YEMEK];
    int yemekSayisi = 0;

    while (fscanf(file, "%s %lf %d %s", yemekler[yemekSayisi].yemekAdi, &yemekler[yemekSayisi].fiyat,
                  &yemekler[yemekSayisi].hazirlamaSuresi, yemekler[yemekSayisi].durum) != EOF) {
        yemekSayisi++;
    }

    fclose(file);

    char guncellenecekYemek[50];
    printf("Guncellenecek Yemek Adi: ");
    scanf("%s", guncellenecekYemek);

    int bulundu = 0;
    for (int i = 0; i < yemekSayisi; i++) {
        if (strcmp(yemekler[i].yemekAdi, guncellenecekYemek) == 0) {
            printf("Yeni Fiyat: ");
            scanf("%lf", &yemekler[i].fiyat);
            printf("Yeni Hazirlama Suresi (dakika): ");
            scanf("%d", &yemekler[i].hazirlamaSuresi);
            printf("Yeni Durum (Mevcut/Mevcut Degil): ");
            scanf("%s", yemekler[i].durum);
            bulundu = 1;
            break;
        }
    }

    if (!bulundu) {
        printf("Yemek bulunamadi.\n");
        return;
    }

    file = fopen("yemeklistesi.txt", "w");
    for (int i = 0; i < yemekSayisi; i++) {
        fprintf(file, "%s %.2f %d %s\n", yemekler[i].yemekAdi, yemekler[i].fiyat, yemekler[i].hazirlamaSuresi, yemekler[i].durum);
    }

    fclose(file);
    printf("Yemek basariyla guncellendi.\n");
}

// Yemek silme
void yemekSil() {
    FILE *file = fopen("yemeklistesi.txt", "r");
    if (file == NULL) {
        printf("yemeklistesi.txt dosyasi acilamadi.\n");
        exit(1);
    }

    Yemek yemekler[MAX_YEMEK];
    int yemekSayisi = 0;

    while (fscanf(file, "%s %lf %d %s", yemekler[yemekSayisi].yemekAdi, &yemekler[yemekSayisi].fiyat,
                  &yemekler[yemekSayisi].hazirlamaSuresi, yemekler[yemekSayisi].durum) != EOF) {
        yemekSayisi++;
    }

    fclose(file);

    char silinecekYemek[50];
    printf("Silinecek Yemek Adi: ");
    scanf("%s", silinecekYemek);

    int bulundu = 0;
    FILE *tempFile = fopen("temp.txt", "w");
    for (int i = 0; i < yemekSayisi; i++) {
        if (strcmp(yemekler[i].yemekAdi, silinecekYemek) != 0) {
            fprintf(tempFile, "%s %.2f %d %s\n", yemekler[i].yemekAdi, yemekler[i].fiyat, yemekler[i].hazirlamaSuresi, yemekler[i].durum);
        } else {
            bulundu = 1;
        }
    }

    fclose(tempFile);

    if (!bulundu) {
        printf("Yemek bulunamadi.\n");
        remove("temp.txt");
        return;
    }

    remove("yemeklistesi.txt");
    rename("temp.txt", "yemeklistesi.txt");
    printf("Yemek basariyla silindi.\n");
}

// Yeni sipariş onaylama/red
void yeniSiparisOnay() {
    FILE *file = fopen("siparisler.txt", "r+");
    if (file == NULL) {
        printf("siparisler.txt dosyasi acilamadi.\n");
        exit(1);
    }

    Siparis siparisler[MAX_SIPARIS];
    int siparisSayisi = 0;

    while (fscanf(file, "%s %s %lf %s %s %s %s", siparisler[siparisSayisi].siparisID, siparisler[siparisSayisi].yemekAdi,
                  &siparisler[siparisSayisi].fiyat, siparisler[siparisSayisi].siparisZamani, siparisler[siparisSayisi].hazirlamaZamani,
                  siparisler[siparisSayisi].kullaniciAdi, siparisler[siparisSayisi].asci) != EOF) {
        siparisSayisi++;
    }

    fclose(file);

    for (int i = 0; i < siparisSayisi; i++) {
        if (strcmp(siparisler[i].hazirlamaZamani, "Bekleniyor") == 0) {
            printf("Siparis ID: %s\nYemek Adi: %s\nFiyat: %.2f\nKullanici: %s\nOnayla (E/H)? ",
                   siparisler[i].siparisID, siparisler[i].yemekAdi, siparisler[i].fiyat, siparisler[i].kullaniciAdi);

            char onay;
            scanf(" %c", &onay);

            if (onay == 'E' || onay == 'e') {
                strcpy(siparisler[i].hazirlamaZamani, "Onaylandi");
            } else {
                strcpy(siparisler[i].hazirlamaZamani, "Reddedildi");
            }
        }
    }

    file = fopen("siparisler.txt", "w");
    for (int i = 0; i < siparisSayisi; i++) {
        fprintf(file, "%s %s %.2f %s %s %s %s\n", siparisler[i].siparisID, siparisler[i].yemekAdi, siparisler[i].fiyat,
                siparisler[i].siparisZamani, siparisler[i].hazirlamaZamani, siparisler[i].kullaniciAdi, siparisler[i].asci);
    }

    fclose(file);

    printf("Tum siparisler islenmistir.\n");
}

// Günlük rapor oluşturma
void gunlukRapor() {
    FILE *file = fopen("siparisler.txt", "r");
    if (file == NULL) {
        printf("siparisler.txt dosyasi acilamadi.\n");
        exit(1);
    }

    char tarih[11];
    printf("Rapor alinacak tarih (dd.mm.yyyy): ");
    scanf("%s", tarih);

    char raporDosyaAdi[20];
    sprintf(raporDosyaAdi, "rapor_%s.txt", tarih);

    FILE *raporFile = fopen(raporDosyaAdi, "w");
    if (raporFile == NULL) {
        printf("rapor dosyasi olusturulamadi.\n");
        fclose(file);
        exit(1);
    }

    Siparis siparis;
    while (fscanf(file, "%s %s %lf %s %s %s %s", siparis.siparisID, siparis.yemekAdi, &siparis.fiyat,
                  siparis.siparisZamani, siparis.hazirlamaZamani, siparis.kullaniciAdi, siparis.asci) != EOF) {
        if (strncmp(siparis.siparisZamani, tarih, 10) == 0) {
            fprintf(raporFile, "%s %s %.2f %s %s %s %s\n", siparis.siparisID, siparis.yemekAdi, siparis.fiyat,
                    siparis.siparisZamani, siparis.hazirlamaZamani, siparis.kullaniciAdi, siparis.asci);
        }
    }

    fclose(file);
    fclose(raporFile);

    printf("Gunluk rapor %s dosyasina kaydedildi.\n", raporDosyaAdi);
}

// Analizler
void analizler() {
    FILE *file = fopen("siparisler.txt", "r");
    if (file == NULL) {
        printf("siparisler.txt dosyasi acilamadi.\n");
        exit(1);
    }

    // Analiz seçenekleri
    printf("1. Gunluk Kazanc\n");
    printf("2. Aylik Kazanc\n");
    printf("3. Donem Kazanci\n");
    printf("4. En Cok Tuketim\n");

    int secim;
    printf("Seciminiz: ");
    scanf("%d", &secim);

    if (secim == 1) {
        char tarih[11];
        printf("Tarih (dd.mm.yyyy): ");
        scanf("%s", tarih);

        double toplamKazanc = 0;
        Siparis siparis;

        while (fscanf(file, "%s %s %lf %s %s %s %s", siparis.siparisID, siparis.yemekAdi, &siparis.fiyat,
                      siparis.siparisZamani, siparis.hazirlamaZamani, siparis.kullaniciAdi, siparis.asci) != EOF) {
            if (strncmp(siparis.siparisZamani, tarih, 10) == 0) {
                toplamKazanc += siparis.fiyat;
            }
        }

        printf("Gunluk kazanc: %.2f TL\n", toplamKazanc);

    } else if (secim == 2) {
        int ay, yil;
        printf("Ay ve Yil (mm yyyy): ");
        scanf("%d %d", &ay, &yil);

        double toplamKazanc = 0;
        Siparis siparis;

        while (fscanf(file, "%s %s %lf %s %s %s %s", siparis.siparisID, siparis.yemekAdi, &siparis.fiyat,
                      siparis.siparisZamani, siparis.hazirlamaZamani, siparis.kullaniciAdi, siparis.asci) != EOF) {
            int siparisAy, siparisYil;
            sscanf(siparis.siparisZamani + 3, "%d.%d", &siparisAy, &siparisYil);
            if (siparisAy == ay && siparisYil == yil) {
                toplamKazanc += siparis.fiyat;
            }
        }

        printf("Aylik kazanc: %.2f TL\n", toplamKazanc);

    } else if (secim == 3) {
        char baslangicTarihi[11], bitisTarihi[11];
        printf("Baslangic Tarihi (dd.mm.yyyy): ");
        scanf("%s", baslangicTarihi);
        printf("Bitis Tarihi (dd.mm.yyyy): ");
        scanf("%s", bitisTarihi);

        double toplamKazanc = 0;
        Siparis siparis;

        while (fscanf(file, "%s %s %lf %s %s %s %s", siparis.siparisID, siparis.yemekAdi, &siparis.fiyat,
                      siparis.siparisZamani, siparis.hazirlamaZamani, siparis.kullaniciAdi, siparis.asci) != EOF) {
            if (strcmp(siparis.siparisZamani, baslangicTarihi) >= 0 && strcmp(siparis.siparisZamani, bitisTarihi) <= 0) {
                toplamKazanc += siparis.fiyat;
            }
        }

        printf("Donem kazanci: %.2f TL\n", toplamKazanc);

    } else if (secim == 4) {
        // En çok tüketim analizi
        printf("En Cok Tuketilen Yemek Analizi\n");

    } else {
        printf("Gecersiz secim.\n");
    }

    fclose(file);
}

// Aşçı sayısı belirleme
void asciSayisiBelirle() {
    int asciSayisi;
    printf("Asci sayisi: ");
    scanf("%d", &asciSayisi);

    FILE *file = fopen("asciSayisi.txt", "w");
    if (file == NULL) {
        printf("asciSayisi.txt dosyasi acilamadi.\n");
        exit(1);
    }

    fprintf(file, "%d\n", asciSayisi);
    fclose(file);

    printf("Asci sayisi basariyla belirlendi.\n");
}

// Ana fonksiyon
int main() {
    int secim;
    while (1) {
        printf("\nRestoran Yonetim Sistemi\n");
        printf("1. Yemek Ekle\n");
        printf("2. Yemek Guncelle\n");
        printf("3. Yemek Sil\n");
        printf("4. Yeni Siparis Onay/Red\n");
        printf("5. Gunluk Rapor\n");
        printf("6. Analizler\n");
        printf("7. Asci Sayisi Belirleme\n");
        printf("8. Cikis\n");

        printf("Seciminiz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                yemekEkle();
                break;
            case 2:
                yemekGuncelle();
                break;
            case 3:
                yemekSil();
                break;
            case 4:
                yeniSiparisOnay();
                break;
            case 5:
                gunlukRapor();
                break;
            case 6:
                analizler();
                break;
            case 7:
                asciSayisiBelirle();
                break;
            case 8:
                exit(0);
            default:
                printf("Gecersiz secim.\n");
        }
    }
    return 0;
}
