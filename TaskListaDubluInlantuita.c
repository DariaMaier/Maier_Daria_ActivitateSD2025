#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
} Masina;

typedef struct Nod {
	Masina info;
	struct Nod* urmator;
	struct Nod* precedent;
} Nod;

typedef struct {
	Nod* prim;
	Nod* ultim;
} Lista;

typedef struct NodLS {
	Masina info;
	struct NodLS* urm;
} NodLS;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[] = ",\n";
	fgets(buffer, sizeof(buffer), file);
	char* aux;
	Masina m;
	aux = strtok(buffer, sep); m.id = atoi(aux);
	aux = strtok(NULL, sep); m.nrUsi = atoi(aux);
	aux = strtok(NULL, sep); m.pret = atof(aux);
	aux = strtok(NULL, sep); m.model = malloc(strlen(aux) + 1); strcpy(m.model, aux);
	aux = strtok(NULL, sep); m.numeSofer = malloc(strlen(aux) + 1); strcpy(m.numeSofer, aux);
	aux = strtok(NULL, sep); m.serie = aux[0];
	return m;
}

void afisareMasina(Masina m) {
	printf("Id: %d | Usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
		m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

void adaugaLaInceput(Lista* lista, Masina m) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = m;
	nou->precedent = NULL;
	nou->urmator = lista->prim;
	if (lista->prim) lista->prim->precedent = nou;
	else lista->ultim = nou;
	lista->prim = nou;
}

Lista citireLista(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Lista lista = { NULL, NULL };
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaLaInceput(&lista, m);
	}
	fclose(f);
	return lista;
}

void afisareListaCrescator(Lista lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->urmator;
	}
}

void afisareListaDescrescator(Lista lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->precedent;
	}
}

void dezalocareListaDubla(Lista* lista) {
	Nod* p = lista->prim;
	while (p) {
		Nod* aux = p;
		p = p->urmator;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
	lista->prim = lista->ultim = NULL;
}

float calculeazaPretMediu(Lista lista) {
	Nod* p = lista.prim;
	float suma = 0;
	int nr = 0;
	while (p) {
		suma += p->info.pret;
		nr++;
		p = p->urmator;
	}
	return nr > 0 ? suma / nr : 0;
}

void stergePozitie(Lista* lista, int poz) {
	if (poz < 0 || !lista->prim) return;
	Nod* p = lista->prim;
	int i = 0;
	while (p && i < poz) {
		p = p->urmator;
		i++;
	}
	if (!p) return;
	if (p->precedent) p->precedent->urmator = p->urmator;
	else lista->prim = p->urmator;
	if (p->urmator) p->urmator->precedent = p->precedent;
	else lista->ultim = p->precedent;
	free(p->info.model);
	free(p->info.numeSofer);
	free(p);
}

void inserareSortata(Lista* lista, Masina m) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = m;
	nou->precedent = NULL;
	nou->urmator = NULL;
	if (!lista->prim || m.pret < lista->prim->info.pret) {
		nou->urmator = lista->prim;
		if (lista->prim) lista->prim->precedent = nou;
		else lista->ultim = nou;
		lista->prim = nou;
		return;
	}
	Nod* p = lista->prim;
	while (p->urmator && p->urmator->info.pret < m.pret) {
		p = p->urmator;
	}
	nou->urmator = p->urmator;
	nou->precedent = p;
	if (p->urmator) p->urmator->precedent = nou;
	else lista->ultim = nou;
	p->urmator = nou;
}

NodLS* copieDeepListaSimpla(Lista lista, float prag) {
	NodLS* cap = NULL, * ultim = NULL;
	Nod* p = lista.prim;
	while (p) {
		if (p->info.pret > prag) {
			Masina m;
			m.id = p->info.id;
			m.nrUsi = p->info.nrUsi;
			m.pret = p->info.pret;
			m.model = malloc(strlen(p->info.model) + 1);
			strcpy(m.model, p->info.model);
			m.numeSofer = malloc(strlen(p->info.numeSofer) + 1);
			strcpy(m.numeSofer, p->info.numeSofer);
			m.serie = p->info.serie;
			NodLS* nou = malloc(sizeof(NodLS));
			nou->info = m;
			nou->urm = NULL;
			if (!cap) cap = ultim = nou;
			else {
				ultim->urm = nou;
				ultim = nou;
			}
		}
		p = p->urmator;
	}
	return cap;
}

void afisareListaSimpla(NodLS* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->urm;
	}
}

void dezalocareListaSimpla(NodLS** cap) {
	while (*cap) {
		NodLS* aux = *cap;
		*cap = (*cap)->urm;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
}

void interschimbaPozitii(Lista lista, int poz1, int poz2) {
	if (poz1 == poz2) return;
	Nod* p1 = lista.prim, * p2 = lista.prim;
	int i = 0;
	while (p1 && i < poz1) { p1 = p1->urmator; i++; }
	i = 0;
	while (p2 && i < poz2) { p2 = p2->urmator; i++; }
	if (p1 && p2) {
		Masina aux = p1->info;
		p1->info = p2->info;
		p2->info = aux;
	}
}

int main() {
	Lista lista = citireLista("masini.txt");

	printf("\n--- Lista crescator:\n");
	afisareListaCrescator(lista);

	printf("\nPret mediu: %.2f\n", calculeazaPretMediu(lista));

	stergePozitie(&lista, 1);
	printf("\n--- Dupa stergere poz 1:\n");
	afisareListaCrescator(lista);

	Masina m;
	m.id = 501;
	m.nrUsi = 4;
	m.pret = 42000;
	m.model = (char*)malloc(strlen("Audi") + 1);
	strcpy(m.model, "Audi");
	m.numeSofer = (char*)malloc(strlen("Daria") + 1);
	strcpy(m.numeSofer, "Daria");
	m.serie = 'D';
	inserareSortata(&lista, m);

	printf("\n--- Dupa inserare sortata (Audi - Daria):\n");
	afisareListaCrescator(lista);

	printf("\n--- Parcurgere descrescator:\n");
	afisareListaDescrescator(lista);

	NodLS* listaSimpla = copieDeepListaSimpla(lista, 10000);
	printf("\n--- Lista simpla (pret > 10000):\n");
	afisareListaSimpla(listaSimpla);

	interschimbaPozitii(lista, 0, 2);
	printf("\n--- Dupa interschimbare pozitii 0 si 2:\n");
	afisareListaCrescator(lista);

	dezalocareListaSimpla(&listaSimpla);
	dezalocareListaDubla(&lista);

	return 0;
}
