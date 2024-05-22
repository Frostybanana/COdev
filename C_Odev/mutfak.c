#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIPARIS 100
#define MAX_ASCI 3

typedef struct {
    char siparisID[20];
    char yemekAdi[50];
    double fiyat;
    char siparisZamani[20];
    char hazirlamaZamani[20];
    char kullaniciAdi[20];
    char asci[5];
    int hazirlamaSuresi;
} Siparis;

// Fonksiyon Prototipleri
void siparisHazirla();
void hazirSiparisler();
void asciAtaVeHazirla(Siparis *siparisler, int siparisSayisi, int asciSayisi);

// Aşçılar için sıra kuyruğu
typedef struct {
    int id;
    int bosZaman;  // Aşçının ne zaman boş olacağı (hazırlama sürelerinin bitiş zamanına göre)
} Asci;

Asci ascilar[MAX_ASCI];

void initializeAscilar() {
    for (int i = 0; i < MAX_ASCI; i++) {
        ascilar[i].id = i + 1;
        ascilar[i].bosZaman = 0;
    }
}

int enErkenBosAsci() {
    int minIndex = 0;
    for (int i = 1; i < MAX_ASCI; i++) {
        if (ascilar[i].bosZaman < ascilar[minIndex].bosZaman) {
            minIndex = i;
        }
    }
    return minIndex;
}

// Sipariş hazırlama
void siparisHazirla() {
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
        // Eğer sipariş bekleniyorsa
        if (strcmp(siparisler[siparisSayisi].hazirlamaZamani, "Bekleniyor") == 0) {
            siparisSayisi++;
        }
    }

    fclose(file);

    if (siparisSayisi == 0) {
        printf("Hazirlanacak siparis yok.\n");
        return;
    }

    asciAtaVeHazirla(siparisler, siparisSayisi, MAX_ASCI);

    file = fopen("siparisler.txt", "w");
    for (int i = 0; i < siparisSayisi; i++) {
        fprintf(file, "%s %s %.2f %s %s %s %s\n", siparisler[i].siparisID, siparisler[i].yemekAdi, siparisler[i].fiyat,
                siparisler[i].siparisZamani, siparisler[i].hazirlamaZamani, siparisler[i].kullaniciAdi, siparisler[i].asci);
    }

    fclose(file);

    printf("Tum siparisler hazirlanmistir.\n");
}

// Siparişlere aşçı ata ve hazırlama süresini güncelle
void asciAtaVeHazirla(Siparis *siparisler, int siparisSayisi, int asciSayisi) {
    FILE *yemekListesiFile = fopen("yemeklistesi.txt", "r");
    if (yemekListesiFile == NULL) {
        printf("yemeklistesi.txt dosyasi acilamadi.\n");
        exit(1);
    }

    char yemekAdi[50];
    double fiyat;
    int hazirlamaSuresi;
    char durum[10];

    while (fscanf(yemekListesiFile, "%s %lf %d %s", yemekAdi, &fiyat, &hazirlamaSuresi, durum) != EOF) {
        for (int i = 0; i < siparisSayisi; i++) {
            if (strcmp(siparisler[i].yemekAdi, yemekAdi) == 0) {
                siparisler[i].hazirlamaSuresi = hazirlamaSuresi;
            }
        }
    }

    fclose(yemekListesiFile);

    initializeAscilar();

    for (int i = 0; i < siparisSayisi; i++) {
        int asciIndex = enErkenBosAsci();
        ascilar[asciIndex].bosZaman += siparisler[i].hazirlamaSuresi;

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        tm.tm_min += ascilar[asciIndex].bosZaman;

        strftime(siparisler[i].hazirlamaZamani, sizeof(siparisler[i].hazirlamaZamani), "%d.%m.%Y-%H:%M", &tm);
        sprintf(siparisler[i].asci, "A%d", ascilar[asciIndex].id);
    }
}

// Hazır siparişler
void hazirSiparisler() {
    FILE *file = fopen("siparisler.txt", "r");
    if (file == NULL) {
        printf("siparisler.txt dosyasi acilamadi.\n");
        exit(1);
    }

    Siparis siparis;
    int found = 0;

    while (fscanf(file, "%s %s %lf %s %s %s %s", siparis.siparisID, siparis.yemekAdi, &siparis.fiyat, siparis.siparisZamani,
                  siparis.hazirlamaZamani, siparis.kullaniciAdi, siparis.asci) != EOF) {
        if (strcmp(siparis.hazirlamaZamani, "Bekleniyor") != 0) {
            printf("Siparis ID: %s\nYemek Adi: %s\nFiyat: %.2f\nSiparis Zamani: %s\nHazirlama Zamani: %s\nAsci: %s\n\n",
                   siparis.siparisID, siparis.yemekAdi, siparis.fiyat, siparis.siparisZamani, siparis.hazirlamaZamani, siparis.asci);
            found = 1;
        }
    }

    if (!found) {
        printf("Hazir siparis yok.\n");
    }

    fclose(file);
}

int main() {
    int choice;

    do {
        printf("1. Siparis Hazirla\n");
        printf("2. Hazir Siparisler\n");
        printf("3. Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                siparisHazirla();
                break;
            case 2:
                hazirSiparisler();
                break;
            case 3:
                printf("Cikis yapiliyor...\n");
                break;
            default:
                printf("Gecersiz secim.\n");
                break;
        }
    } while (choice != 3);

    return 0;
}
