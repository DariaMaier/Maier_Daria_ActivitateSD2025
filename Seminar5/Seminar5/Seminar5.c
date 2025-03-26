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

struct Nod {
	Masina info;
	struct Nod* urmator;
	struct Nod* precedent;
};
typedef struct Nod Nod;


struct Lista {
	Nod* prim;
	Nod* ultim;
};
typedef struct Lista Lista;

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
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Lista list) {
	Nod* p = list.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->urmator;
	}

}
void afisareListaMasiniSf(Lista list) {
	Nod* p = list.ultim;

}

void adaugaMasinaInLista(Lista* list, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->precedent = list->ultim;
	p->urmator = NULL;
	if (list->ultim) {
		list->ultim->urmator = p;
	}
	else {
		list->prim = p;
	}
	list->ultim = p;
}

void adaugaLaInceputInLista(Lista* list, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->urmator = list->prim;
	p->precedent = NULL;

	if (list->prim) {
		list->prim->precedent = p;
	}
	else {
		list->ultim = p;
	}
	list->prim = p;

}

Lista citireLDMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	Lista list;
	list.prim = NULL;
	list.ultim = NULL;
	while (!feof(f)) {
		adaugaLaInceputInLista(&list, citireMasinaDinFisier(f));
	}
	return list;
	fclose(f);
}

void dezalocareLDMasini(Lista* list) {
	Nod* p = list->prim;
	while (p->urmator) {
		free(p->info.numeSofer);
		free(p->info.model);
		p = p->urmator;
		free(p->precedent);
	}
	
	free(p->info.numeSofer);
	free(p->info.model);
	free(p);
	list->prim = NULL;
	list->ultim = NULL;
}

float calculeazaPretMediu(Lista lista) {

	Nod* aux = lista.prim;
	float suma = 0;
	float cnt = 0;
	while (aux) {
		suma += aux->info.pret;
		cnt++;
		aux = aux->urmator;
	}

	return suma / cnt;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	return NULL;
}

int main() {
	Lista lista;
	lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);

	float pretMediu = calculeazaPretMediu(lista);
	printf("Pret mediu: %.2f\n", pretMediu);

	dezalocareLDMasini(&lista);

	return 0;
}