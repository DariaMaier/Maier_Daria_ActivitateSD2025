#include <stdio.h>
#include<malloc.h>


struct Masina {
    int id;
    int anFabricatie;
    char* sofer;
    float kilometriiParcursi;
    char initialaProducator;
};

struct Masina initializare(int id, int anFabricatie, const char* sofer, float kilometriiParcursi, char initialaProducator) {
    struct Masina m;
    m.id = id;
    m.anFabricatie = anFabricatie;

    if (sofer != NULL) {
        m.sofer = (char*)malloc(strlen(sofer) + 1);
        if (m.sofer != NULL) { 
            strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
        }
    }
    else {
        m.sofer = NULL;
    }

    m.kilometriiParcursi = kilometriiParcursi;
    m.initialaProducator = initialaProducator;
    return m;
}

void afisareVector(struct Masina* vector, int nrElemente) {
    if (vector != NULL && nrElemente > 0) {
        for (int i = 0; i < nrElemente; i++) {
            printf("ID: %d \n", vector[i].id);
            printf("An fabricatie: %d \n", vector[i].anFabricatie);
            printf("Sofer: %s \n", (vector[i].sofer != NULL) ? vector[i].sofer : "N/A");
            printf("Kilometrii Parcursi : %.2f \n", vector[i].kilometriiParcursi);
            printf("Initiala producator: %c \n\n", vector[i].initialaProducator);
        }
    }
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
    struct Masina* vectorNou = NULL;
    if (nrElementeCopiate > 0 && nrElementeCopiate <= nrElemente) {
        vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * nrElementeCopiate);
        if (vectorNou != NULL) {
            for (int i = 0; i < nrElementeCopiate; i++) {
                vectorNou[i] = initializare(vector[i].id, vector[i].anFabricatie, vector[i].sofer, vector[i].kilometriiParcursi, vector[i].initialaProducator);
            }
        }
    }
    return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
    if (vector != NULL && *vector != NULL && nrElemente != NULL && *nrElemente > 0) {
        for (int i = 0; i < (*nrElemente); i++) {
            free((*vector)[i].sofer);
        }
        free(*vector);
        *vector = NULL;
        *nrElemente = 0;
    }
}

void copiazaMasiniCuMultiKm(struct Masina* vector, int nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
    int contor = 0;
    if (vector != NULL && nrElemente > 0) {
        (*dimensiune) = 0;
        for (int i = 0; i < nrElemente; i++) {
            if (vector[i].kilometriiParcursi > prag) {
                (*dimensiune)++;
            }
        }
        *vectorNou = malloc(sizeof(struct Masina) * (*dimensiune));
        for (int i = 0; i < nrElemente; i++) {
            if (vector[i].kilometriiParcursi > prag) {
                (*vectorNou)[contor] = vector[i];
                (*vectorNou)[contor].sofer = malloc(sizeof(char) * (strlen(vector[i].sofer) + 1));
                strcpy_s((*vectorNou)[contor].sofer, strlen(vector[i].sofer) + 1, vector[i].sofer);
                contor++;
            }
        }
    }

}

struct Masina getPrimaMasinaDupaSofer(struct Masina* vector, int nrElemente, const char* soferCautat) {
    if (vector != NULL && soferCautat != NULL) {
        for (int i = 0; i < nrElemente; i++) {
            if (vector[i].sofer != NULL && strcmp(soferCautat, vector[i].sofer) == 0) {
                return vector[i];
            }
        }
    }
    return initializare(-1, 0, NULL, 0, '-');
}

int main() {
    int nrElemente = 3;
    struct Masina* vector = (struct Masina*)malloc(sizeof(struct Masina) * nrElemente);

    vector[0] = initializare(1, 2010, "Marian", 2100, 'F');
    vector[1] = initializare(22, 2008, "Cristi", 1500.6, 'A');
    vector[2] = initializare(3, 2017, "Stefan", 7500, 'M');

    printf("Masini initiale:\n");
    afisareVector(vector, nrElemente);

    int nrElementeCopiate = 2;
    struct Masina* vectorNou = copiazaPrimeleNElemente(vector, nrElemente, nrElementeCopiate);

    printf("Masini copiate:\n");
    if (vectorNou != NULL) {
        afisareVector(vectorNou, nrElementeCopiate);
        dezalocare(&vectorNou, &nrElementeCopiate);
    }

    float prag = 2000;
    copiazaMasiniCuMultiKm(vector, nrElemente, prag, &vectorNou, &nrElementeCopiate);

    printf("Masini cu mai mult de %.2f km:\n", prag);
    if (vectorNou != NULL) {
        afisareVector(vectorNou, nrElementeCopiate);
        dezalocare(&vectorNou, &nrElementeCopiate);
    }

    printf("Prima masina a lui Marian:\n");
    struct Masina primaMasina = getPrimaMasinaDupaSofer(vector, nrElemente, "Marian");
    afisareVector(&primaMasina, 1); 

    dezalocare(&vector, &nrElemente);

    return 0;
}
