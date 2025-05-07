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
    printf("ID: %d | Piese: %d | Suprafata: %.2f mp | Culoare: %s\n", d.id, d.nrPiese, d.suprafata, d.culoare);
}

void dezalocareDormitor(Dormitor* d) {
    free(d->culoare);
    d->culoare = NULL;
}

void afisareVectorDormitoare(Dormitor* vector, int dim) {
    for (int i = 0; i < dim; i++) {
        afisareDormitor(vector[i]);
    }
}


Dormitor* copiazaDormitoareMari(Dormitor* vector, int dim, int* dimNoua) {
    Dormitor* nou = NULL;
    *dimNoua = 0;
    for (int i = 0; i < dim; i++) {
        float spatiu = vector[i].suprafata / vector[i].nrPiese;
        if (spatiu > 10) {
            nou = (Dormitor*)realloc(nou, (*dimNoua + 1) * sizeof(Dormitor));
            nou[*dimNoua] = vector[i];
            (*dimNoua)++;
        }
    }
    return nou;
}


Dormitor* mutaDormitoareMici(Dormitor** vector, int* dim, int* dimNoua) {
    Dormitor* nou = NULL;
    *dimNoua = 0;
    int i = 0;
    while (i < *dim) {
        if ((*vector)[i].nrPiese < 3) {
            nou = (Dormitor*)realloc(nou, (*dimNoua + 1) * sizeof(Dormitor));
            nou[*dimNoua] = (*vector)[i];
            (*dimNoua)++;

            for (int j = i; j < *dim - 1; j++) {
                (*vector)[j] = (*vector)[j + 1];
            }
            (*dim)--;
            *vector = (Dormitor*)realloc(*vector, (*dim) * sizeof(Dormitor));
        }
        else {
            i++;
        }
    }
    return nou;
}


Dormitor* concateneazaVectori(Dormitor* v1, int dim1, Dormitor* v2, int dim2, int* dimNou) {
    *dimNou = dim1 + dim2;
    Dormitor* rezultat = (Dormitor*)malloc((*dimNou) * sizeof(Dormitor));
    for (int i = 0; i < dim1; i++) rezultat[i] = v1[i];
    for (int i = 0; i < dim2; i++) rezultat[dim1 + i] = v2[i];
    return rezultat;
}


void dezalocareVector(Dormitor* vector, int dim) {
    for (int i = 0; i < dim; i++) {
        dezalocareDormitor(&vector[i]);
    }
    free(vector);
}

int main() {
    int n = 5;
    Dormitor* vector = (Dormitor*)malloc(n * sizeof(Dormitor));
    printf("Introduceti 5 dormitoare:\n");
    for (int i = 0; i < n; i++) {
        printf("\n--- Dormitor %d ---\n", i + 1);
        vector[i] = citireDormitor();
    }

    printf("\n==== Toate dormitoarele ====\n");
    afisareVectorDormitoare(vector, n);

    int dimCopiate = 0;
    Dormitor* mari = copiazaDormitoareMari(vector, n, &dimCopiate);
    printf("\n==== Dormitoare CU SPATIU > 10mp/piesa ====\n");
    afisareVectorDormitoare(mari, dimCopiate);

    int dimMutate = 0;
    Dormitor* mici = mutaDormitoareMici(&vector, &n, &dimMutate);
    printf("\n==== Dormitoare MUTATE (cu < 3 piese) ====\n");
    afisareVectorDormitoare(mici, dimMutate);

    printf("\n==== Vectorul RAMAS dupa mutare ====\n");
    afisareVectorDormitoare(vector, n);

    int dimFinal = 0;
    Dormitor* concatenat = concateneazaVectori(vector, n, mari, dimCopiate, &dimFinal);
    printf("\n==== Vector CONCATENAT ====\n");
    afisareVectorDormitoare(concatenat, dimFinal);

    dezalocareVector(vector, n);
    dezalocareVector(mari, dimCopiate);
    dezalocareVector(mici, dimMutate);
    dezalocareVector(concatenat, dimFinal);

    return 0;
}
