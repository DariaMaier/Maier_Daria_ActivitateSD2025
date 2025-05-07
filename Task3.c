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

Dormitor creeazaDormitor(int id, int nrPiese, float suprafata, const char* culoare) {
    Dormitor d;
    d.id = id;
    d.nrPiese = nrPiese;
    d.suprafata = suprafata;
    d.culoare = (char*)malloc(strlen(culoare) + 1);
    strcpy(d.culoare, culoare);
    return d;
}

void afisareDormitor(Dormitor d) {
    printf("ID: %d | Piese: %d | Suprafata: %.2f | Culoare: %s\n", d.id, d.nrPiese, d.suprafata, d.culoare);
}

void dezalocareDormitor(Dormitor* d) {
    free(d->culoare);
    d->culoare = NULL;
}

void dezalocareVector(Dormitor* vector, int dim) {
    for (int i = 0; i < dim; i++) {
        dezalocareDormitor(&vector[i]);
    }
    free(vector);
}


Dormitor* citesteDormitoareDinFisier(const char* numeFisier, int* dim) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        *dim = 0;
        return NULL;
    }

    Dormitor* vector = NULL;
    *dim = 0;
    int id, nrPiese;
    float suprafata;
    char culoare[100];

    while (fscanf(f, "%d %d %f %s", &id, &nrPiese, &suprafata, culoare) == 4) {
        vector = (Dormitor*)realloc(vector, (*dim + 1) * sizeof(Dormitor));
        vector[*dim] = creeazaDormitor(id, nrPiese, suprafata, culoare);
        (*dim)++;
    }

    fclose(f);
    return vector;
}

void scrieDormitorInFisier(const char* numeFisier, Dormitor d) {
    FILE* f = fopen(numeFisier, "a");
    if (f) {
        fprintf(f, "%d %d %.2f %s\n", d.id, d.nrPiese, d.suprafata, d.culoare);
        fclose(f);
    }
}


void scrieVectorDormitoare(const char* numeFisier, Dormitor* vector, int dim) {
    FILE* f = fopen(numeFisier, "w");
    if (f) {
        for (int i = 0; i < dim; i++) {
            fprintf(f, "%d %d %.2f %s\n", vector[i].id, vector[i].nrPiese, vector[i].suprafata, vector[i].culoare);
        }
        fclose(f);
    }
}

int main() {
    int dim = 0;
    Dormitor* vector = citesteDormitoareDinFisier("dormitoare.txt", &dim);

    printf("Dormitoare citite din fisier:\n");
    for (int i = 0; i < dim; i++) {
        afisareDormitor(vector[i]);
    }

    Dormitor nou = creeazaDormitor(11, 2, 11.5, "bleu");
    scrieDormitorInFisier("dormitoare.txt", nou);

    scrieVectorDormitoare("dormitoare_salvate.txt", vector, dim);

    dezalocareDormitor(&nou);
    dezalocareVector(vector, dim);

    return 0;
}
