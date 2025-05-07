#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct Nod Nod;

struct Nod {
    Masina info;
    Nod* urm;
};

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char* aux;
    Masina m1;
    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);
    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);
    m1.serie = *strtok(NULL, sep);
    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
    while (cap) {
        afisareMasina(cap->info);
        cap = cap->urm;
    }
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->urm = *cap;
    *cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Nod* lista = NULL;
    while (!feof(f)) {
        Masina m = citireMasinaDinFisier(f);
        adaugaLaInceputInLista(&lista, m);
    }
    fclose(f);
    return lista;
}

void dezalocareListaMasini(Nod** cap) {
    while (*cap != NULL) {
        Nod* aux = *cap;
        *cap = (*cap)->urm;
        free(aux->info.model);
        free(aux->info.numeSofer);
        free(aux);
    }
}

void stergePozitie(Nod** cap, int pozitie) {
    if (!(*cap) || pozitie < 0) return;
    Nod* temp = *cap;
    if (pozitie == 0) {
        *cap = temp->urm;
        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp);
        return;
    }
    for (int i = 0; temp && i < pozitie - 1; i++) {
        temp = temp->urm;
    }
    if (!temp || !(temp->urm)) return;
    Nod* nodDeSters = temp->urm;
    temp->urm = nodDeSters->urm;
    free(nodDeSters->info.model);
    free(nodDeSters->info.numeSofer);
    free(nodDeSters);
}

void inserareSortata(Nod** cap, Masina m) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = m;
    nou->urm = NULL;
    if (!(*cap) || m.pret < (*cap)->info.pret) {
        nou->urm = *cap;
        *cap = nou;
        return;
    }
    Nod* temp = *cap;
    while (temp->urm && temp->urm->info.pret < m.pret) {
        temp = temp->urm;
    }
    nou->urm = temp->urm;
    temp->urm = nou;
}

Masina* vectorMasiniFiltrate(Nod* cap, int* dim) {
    *dim = 0;
    Nod* temp = cap;
    while (temp) {
        if (temp->info.nrUsi > 2) (*dim)++;
        temp = temp->urm;
    }
    if (*dim == 0) return NULL;
    Masina* vector = (Masina*)malloc(sizeof(Masina) * (*dim));
    temp = cap;
    int i = 0;
    while (temp) {
        if (temp->info.nrUsi > 2) {
            vector[i].id = temp->info.id;
            vector[i].nrUsi = temp->info.nrUsi;
            vector[i].pret = temp->info.pret;
            vector[i].model = (char*)malloc(strlen(temp->info.model) + 1);
            strcpy(vector[i].model, temp->info.model);
            vector[i].numeSofer = (char*)malloc(strlen(temp->info.numeSofer) + 1);
            strcpy(vector[i].numeSofer, temp->info.numeSofer);
            vector[i].serie = temp->info.serie;
            i++;
        }
        temp = temp->urm;
    }
    return vector;
}

void interschimbaPozitii(Nod* cap, int poz1, int poz2) {
    if (poz1 == poz2 || poz1 < 0 || poz2 < 0) return;
    if (poz1 > poz2) {
        int aux = poz1;
        poz1 = poz2;
        poz2 = aux;
    }
    Nod* temp = cap;
    Nod* nod1 = NULL;
    Nod* nod2 = NULL;
    int index = 0;
    while (temp) {
        if (index == poz1) nod1 = temp;
        if (index == poz2) nod2 = temp;
        temp = temp->urm;
        index++;
    }
    if (nod1 && nod2) {
        Masina aux = nod1->info;
        nod1->info = nod2->info;
        nod2->info = aux;
    }
}

int main() {
    Nod* cap = citireListaMasiniDinFisier("masini.txt");

    printf("\n--- Lista initiala ---\n");
    afisareListaMasini(cap);

    printf("\n--- Dupa stergere la pozitia 2 ---\n");
    stergePozitie(&cap, 2);
    afisareListaMasini(cap);

    printf("\n--- Inserare sortata dupa pret ---\n");
    Masina mNou;
    mNou.id = 100;
    mNou.nrUsi = 4;
    mNou.pret = 75000.0f;
    mNou.model = (char*)malloc(strlen("Tesla") + 1);
    strcpy(mNou.model, "Tesla");
    mNou.numeSofer = (char*)malloc(strlen("Daria") + 1);
    strcpy(mNou.numeSofer, "Daria");
    mNou.serie = 'T';
    inserareSortata(&cap, mNou);
    afisareListaMasini(cap);

    printf("\n--- Vector masini cu >2 usi ---\n");
    int dim = 0;
    Masina* vector = vectorMasiniFiltrate(cap, &dim);
    for (int i = 0; i < dim; i++) {
        afisareMasina(vector[i]);
        free(vector[i].model);
        free(vector[i].numeSofer);
    }
    free(vector);

    printf("\n--- Interschimbare pozitii 0 si 1 ---\n");
    interschimbaPozitii(cap, 0, 1);
    afisareListaMasini(cap);

    dezalocareListaMasini(&cap);
    return 0;
}
