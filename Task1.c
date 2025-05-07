#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Dormitor {
    int id;
    int nrPiese;
    float suprafata;
    char* culoare;
};
typedef struct Dormitor Dormitor;

Dormitor citireDormitor() {
    Dormitor d;
    char buffer[100];

    printf("ID: ");
    scanf("%d", &d.id);

    printf("Numar piese: ");
    scanf("%d", &d.nrPiese);

    printf("Suprafata (mp): ");
    scanf("%f", &d.suprafata);
    getchar(); 

    printf("Culoare: ");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0; 

    d.culoare = (char*)malloc(strlen(buffer) + 1);
    strcpy(d.culoare, buffer);

    return d;
}

void afisareDormitor(Dormitor d) {
    printf("\n--- Informatii dormitor ---\n");
    printf("ID: %d\n", d.id);
    printf("Numar piese: %d\n", d.nrPiese);
    printf("Suprafata: %.2f mp\n", d.suprafata);
    printf("Culoare: %s\n", d.culoare);
}

float calculeazaSpatiuPerPiesa(Dormitor d) {
    if (d.nrPiese == 0) return 0;
    return d.suprafata / d.nrPiese;
}

void modificaCuloare(Dormitor* d, const char* culoareNoua) {
    free(d->culoare);
    d->culoare = (char*)malloc(strlen(culoareNoua) + 1);
    strcpy(d->culoare, culoareNoua);
}

void dezalocareDormitor(Dormitor* d) {
    free(d->culoare);
    d->culoare = NULL;
}

int main() {
    Dormitor d = citireDormitor();
    afisareDormitor(d);

    float spatiu = calculeazaSpatiuPerPiesa(d);
    printf("Spatiu mediu per piesa: %.2f mp\n", spatiu);

    printf("\nModificam culoarea in 'bej deschis'...\n");
    modificaCuloare(&d, "bej deschis");
    afisareDormitor(d);

    dezalocareDormitor(&d);
    return 0;
}
