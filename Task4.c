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

void dezalocareDormitor(Dormitor* d) {
    free(d->culoare);
    d->culoare = NULL;
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

void afisareDormitor(Dormitor d) {
    printf("ID: %d | Piese: %d | Suprafata: %.2f | Culoare: %s\n", d.id, d.nrPiese, d.suprafata, d.culoare);
}

int gasesteMaxNrPiese(Dormitor* vector, int dim) {
    int max = 0;
    for (int i = 0; i < dim; i++) {
        if (vector[i].nrPiese > max)
            max = vector[i].nrPiese;
    }
    return max;
}

Dormitor** copiazaInMatrice(Dormitor* vector, int dim, int* nrLinii, int** dimLinii) {
    *nrLinii = gasesteMaxNrPiese(vector, dim);
    Dormitor** matrice = (Dormitor**)malloc((*nrLinii) * sizeof(Dormitor*));
    *dimLinii = (int*)calloc(*nrLinii, sizeof(int));

    for (int i = 0; i < *nrLinii; i++) {
        matrice[i] = NULL;
    }

    for (int i = 0; i < dim; i++) {
        int indexLinie = vector[i].nrPiese - 1;
        int poz = (*dimLinii)[indexLinie];
        matrice[indexLinie] = (Dormitor*)realloc(matrice[indexLinie], (poz + 1) * sizeof(Dormitor));
        matrice[indexLinie][poz] = creeazaDormitor(vector[i].id, vector[i].nrPiese, vector[i].suprafata, vector[i].culoare);
        (*dimLinii)[indexLinie]++;
    }

    return matrice;
}

void afisareMatrice(Dormitor** matrice, int nrLinii, int* dimLinii) {
    for (int i = 0; i < nrLinii; i++) {
        if (dimLinii[i] > 0) {
            printf("Cluster %d (nrPiese = %d):\n", i + 1, i + 1);
            for (int j = 0; j < dimLinii[i]; j++) {
                afisareDormitor(matrice[i][j]);
            }
            printf("\n");
        }
    }
}

void sorteazaLiniiDupaDim(Dormitor*** matrice, int* dimLinii, int nrLinii) {
    for (int i = 0; i < nrLinii - 1; i++) {
        for (int j = i + 1; j < nrLinii; j++) {
            if (dimLinii[i] > dimLinii[j]) {
                int auxDim = dimLinii[i];
                dimLinii[i] = dimLinii[j];
                dimLinii[j] = auxDim;

                Dormitor* aux = (*matrice)[i];
                (*matrice)[i] = (*matrice)[j];
                (*matrice)[j] = aux;
            }
        }
    }
}

void dezalocareMatrice(Dormitor** matrice, int nrLinii, int* dimLinii) {
    for (int i = 0; i < nrLinii; i++) {
        for (int j = 0; j < dimLinii[i]; j++) {
            dezalocareDormitor(&matrice[i][j]);
        }
        free(matrice[i]);
    }
    free(matrice);
    free(dimLinii);
}

int main() {
    int dim = 0;
    Dormitor* vector = citesteDormitoareDinFisier("dormitoare.txt", &dim);

    int nrLinii;
    int* dimLinii;
    Dormitor** matrice = copiazaInMatrice(vector, dim, &nrLinii, &dimLinii);

    printf("\nMatrice inainte de sortare:\n");
    afisareMatrice(matrice, nrLinii, dimLinii);

    sorteazaLiniiDupaDim(&matrice, dimLinii, nrLinii);

    printf("Matrice dupa sortarea liniilor (dupa nr. elemente):\n");
    afisareMatrice(matrice, nrLinii, dimLinii);

    dezalocareMatrice(matrice, nrLinii, dimLinii);
    for (int i = 0; i < dim; i++) dezalocareDormitor(&vector[i]);
    free(vector);

    return 0;
}
