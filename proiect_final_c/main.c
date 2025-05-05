#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"


#define MAX_ALBUME 10
#define MAX_FOTOGRAFII 20

struct Fotografie {
    char nume[100];
    int dimensiune;
};

struct Album {
    char nume[100];
    struct Fotografie fotografii[MAX_FOTOGRAFII];
    int nr_fotografii;
};

struct Album albume[MAX_ALBUME];
int nr_albume = 0;

char lista_fotografii[][100] = {"Munte.jpg", "Plaja.jpg", "Apus.jpg", "Padure.png", "Oras.png"};
int dim_foto_disponibile[] = {500, 800, 300, 700, 1000};
int nr_foto_disponibile = sizeof(lista_fotografii) / sizeof(lista_fotografii[0]);

void salvare_in_fisier() {
    FILE *f = fopen("galerie.txt", "w");
    if (!f) {
        printf("Eroare la deschiderea fisierului!\n");
        return;
    }
    fprintf(f, "%d\n", nr_albume);
    for (int i = 0; i < nr_albume; i++) {
        fprintf(f, "%s %d\n", albume[i].nume, albume[i].nr_fotografii);
        for (int j = 0; j < albume[i].nr_fotografii; j++) {
            fprintf(f, "%s %d\n", albume[i].fotografii[j].nume, albume[i].fotografii[j].dimensiune);
        }
    }
    fclose(f);
}

void citire_din_fisier() {
    FILE *f = fopen("galerie.txt", "r");
    if (!f) return;
    fscanf(f, "%d", &nr_albume);
    for (int i = 0; i < nr_albume; i++) {
        fscanf(f, "%s %d", albume[i].nume, &albume[i].nr_fotografii);
        for (int j = 0; j < albume[i].nr_fotografii; j++) {
            fscanf(f, "%s %d", albume[i].fotografii[j].nume, &albume[i].fotografii[j].dimensiune);
        }
    }
    fclose(f);
}

void creeaza_album() {
    printf("Creeaza album nou\n");
    if (nr_albume >= MAX_ALBUME) {
        printf(RED "Numar maxim de albume atins!" RESET "\n");
        return;
    }
    printf("Introduceti numele albumului(fara spatii): ");
   int rezultat = scanf("%99s", albume[nr_albume].nume);


    char c;
    int exista_spatiu = 0;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (!isspace(c)) {
            exista_spatiu = 1;
        }
    }

    if (exista_spatiu) {
        printf(RED "Eroare: Numele albumului nu trebuie sa contina spatii!" RESET "\n");
        return;
    }
    for (int i = 0; i < nr_albume; i++) {
        if (strcmp(albume[i].nume, albume[nr_albume].nume) == 0) {
            printf(RED "Eroare: Exista deja un album cu acest nume!" RESET "\n");
            return;
        }
    }


    albume[nr_albume].nr_fotografii = 0;
    nr_albume++;
    salvare_in_fisier();
}

void adauga_fotografie() {
     printf("Adauga fotografie intr-un album \n");
    if (nr_albume == 0) {
        printf(RED "Nu exista albume! Creeaza mai intai un album." RESET "\n");
        return;
    }

    printf("-= Albume existente =-\n");
    for (int i = 0; i < nr_albume; i++)
        printf("%d. " MAGENTA " %s" RESET "\n", i + 1, albume[i].nume);

    int alegere = 0;
    printf("Alege albumul(ex: 1): ");
    scanf("%d", &alegere);
    if (alegere < 1 || alegere > nr_albume) {
        printf(RED "Eroare: Index invalid! Nu exista un album cu acest numar." RESET "\n");
        return;
    }

    alegere--;

    if (albume[alegere].nr_fotografii >= MAX_FOTOGRAFII) {
        printf("Albumul este plin.\n");
        return;
    }

    printf("-= Fotografii disponibile =-\n");
    for (int i = 0; i < nr_foto_disponibile; i++) {
        printf("%d. " YELLOW "%s " RESET "- " GREEN "%d" RED "KB" RESET "\n", i + 1, lista_fotografii[i], dim_foto_disponibile[i]);
    }

    int alegere_foto = 0;
    printf("Alege o fotografie(ex: 1): ");
    scanf("%d", &alegere_foto);
    if (alegere_foto < 1 || alegere_foto > nr_foto_disponibile) {
        printf(RED "Eroare: Index invalid! Nu exista o fotografie cu acest numar." RESET "\n");
        return;
    }
    alegere_foto--;

    struct Fotografie *foto = &albume[alegere].fotografii[albume[alegere].nr_fotografii];
    strcpy(foto->nume, lista_fotografii[alegere_foto]);
    foto->dimensiune = dim_foto_disponibile[alegere_foto];
    albume[alegere].nr_fotografii++;

    salvare_in_fisier();
}

void afiseaza_galerie() {
    printf("Vizualizeaza albume si dimensiunea lor\n");
    if (nr_albume == 0) {
        printf(RED "Nu exista albume!" RESET "\n");
        return;
    }

    for (int i = 0; i < nr_albume; i++) {
        int total_dim = 0;
        for (int j = 0; j < albume[i].nr_fotografii; j++) {
            total_dim += albume[i].fotografii[j].dimensiune;
        }
    printf("%d. " CYAN "%s" RESET " (" RED "%d " YELLOW " fotografii " RESET ", " BLUE "%d " RESET " KB)\n", i + 1, albume[i].nume, albume[i].nr_fotografii, total_dim);


    }
}

void sterge_album() {

    printf("Sterge un album\n");
    if (nr_albume == 0) {
        printf(RED "Nu exista albume!" RESET "\n");
        return;
    }

    printf("-= Albume existente =-\n");
    for (int i = 0; i < nr_albume; i++) {
        printf("%d. " RED "%s" RESET "\n", i + 1, albume[i].nume);
    }

    int stergere = 0;
    printf("Alege albumul de sters(ex: 1): ");
    scanf("%d", &stergere);
    if (stergere < 1 || stergere > nr_albume) {
        printf(RED "Eroare: Index invalid! Nu exista un album cu acest numar." RESET "\n");
        return;
    }
    stergere--;

    for (int i = stergere; i < nr_albume - 1; i++) {
        albume[i] = albume[i + 1];
    }
    nr_albume--;
    salvare_in_fisier();
}

int main() {
    citire_din_fisier();

    while (1) {
        system("cls");
        printf("-= Sistem galerie foto =-\n");
        printf("1. Creeaza album nou\n");
        printf("2. Adauga fotografie intr-un album\n");
        printf("3. Vizualizeaza albume si dimensiunea lor\n");
        printf("4. Sterge un album\n");
        printf("5. Iesire\n");

        int optiune = 0;
        printf("Introduceti optiunea dorita: ");
        scanf("%d", &optiune);
        system("cls");

        switch (optiune) {
            case 1: creeaza_album(); break;
            case 2: adauga_fotografie(); break;
            case 3: afiseaza_galerie(); break;
            case 4: sterge_album(); break;
            case 5: printf(RED "Iesire din aplicatie." RESET "\n"); return 0;
            default: printf("Optiune invalida!\n"); break;
        }

        printf("\n" MAGENTA "Apasa Enter pentru a continua..." RESET);
        getchar(); getchar();
    }

    return 0;
}
