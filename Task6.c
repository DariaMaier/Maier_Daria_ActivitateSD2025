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
	strcpy(m1.model, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);
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

void afisareListaMasiniInceput(Lista list) {
	Nod* p = list.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->urmator;
	}
}

void afisareListaMasiniSf(Lista list) {
	Nod* p = list.ultim;
	while (p) {
		afisareMasina(p->info);
		p = p->precedent;
	}
}

void adaugaMasinaInLista(Lista* list, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->precedent = list->ultim;
	p->urmator = NULL;
	if (list->ultim) list->ultim->urmator = p;
	else list->prim = p;
	list->ultim = p;
}

void adaugaLaInceputInLista(Lista* list, Masina masinaNoua) {
	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->urmator = list->prim;
	p->precedent = NULL;
	if (list->prim) list->prim->precedent = p;
	else list->ultim = p;
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
	fclose(f);
	return list;
}

void dezalocareLDMasini(Lista* lista) {
	Nod* p = lista->prim;
	while (p) {
		Nod* aux = p;
		p = p->urmator;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
	lista->prim = NULL;
	lista->ultim = NULL;
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
	return cnt > 0 ? suma / cnt : 0;
}


void stergePozitie(Lista* lista, int poz) {
	if (!lista->prim || poz < 0) return;
	Nod* p = lista->prim;
	int index = 0;
	while (p && index < poz) {
		p = p->urmator;
		index++;
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
	Nod* nou = (Nod*)malloc(sizeof(Nod));
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

struct NodLS {
	Masina info;
	struct NodLS* urm;
};
typedef struct NodLS NodLS;

NodLS* deepCopyInListaSimpla(Lista lista, float prag) {
	NodLS* cap = NULL;
	NodLS* ultim = NULL;
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
	Nod* p1 = lista.prim;
	Nod* p2 = lista.prim;
	int i = 0;
	while (p1 && i < poz1) {
		p1 = p1->urmator;
		i++;
	}
	i = 0;
	while (p2 && i < poz2) {
		p2 = p2->urmator;
		i++;
	}
	if (p1 && p2) {
		Masina aux = p1->info;
		p1->info = p2->info;
		p2->info = aux;
	}
}

int main() {
	Lista lista = citireLDMasiniDinFisier("masini.txt");

	printf("Lista crescator:\n");
	afisareListaCrescator(lista);

	printf("Pret mediu: %.2f\n", calculeazaPretMediu(lista));

	stergePozitie(&lista, 1);
	printf("\nDupa stergere la pozitia 1:\n");
	afisareListaCrescator(lista);

	Masina m;
	m.id = 501;
	m.nrUsi = 4;
	m.pret = 42000;
	m.model = strdup("Audi");
	m.numeSofer = strdup("Daria");
	m.serie = 'A';
	inserareSortata(&lista, m);

	printf("\nDupa inserare sortata:\n");
	afisareListaCrescator(lista);

	printf("\nLista descrescator:\n");
	afisareListaDescrescator(lista);

	NodLS* simpla = deepCopyInListaSimpla(lista, 30000);
	printf("\nLista simpla (pret > 30000):\n");
	afisareListaSimpla(simpla);

	interschimbaPozitii(lista, 0, 2);
	printf("\nDupa interschimbare pozitii 0 si 2:\n");
	afisareListaCrescator(lista);

	dezalocareListaSimpla(&simpla);
	dezalocareLDMasini(&lista);
	return 0;
}
