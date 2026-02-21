// classe.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_des_classes.h"

#define FICHIER_CLASSES "classes.csv"


typedef enum {Licence, Master} Niveau;

typedef struct {
    int code;
    char nom[30];
    Niveau niveau;
} Classes;

void ajouterClasse();
void modifierClasse();
void rechercherClasse();
void supprimerClasse();
void afficherClasses();
void menuGestionClasses();


Classes *classe;


// Programme principal
void gestion_des_classes() {
    menuGestionClasses();
    printf("Fin du programme.\n");
    return;
}


void menuGestionClasses() {
    int choix;
    do {
        printf("\n===== GESTION DES CLASSES =====\n");
        printf("1. Ajouter une classe\n");
        printf("2. Modifier une classe\n");
        printf("3. Rechercher une classe\n");
        printf("4. Supprimer une classe\n");
        printf("5. Afficher toutes les classes\n");
        printf("0. Aller vers le menu précédent\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar(); // pour consommer le \n après scanf

        switch (choix) {
            case 1:
                ajouterClasse();
                break;
            case 2:
                modifierClasse();
                break;
            case 3:
                rechercherClasse();
                break;
            case 4:
                supprimerClasse();
                break;
            case 5:
                afficherClasses();
                break;
            case 0:
                printf("Retour au menu principal...\n");
                break;
            default:
                printf(" Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 0);
    return;
}



void ajouterClasse() {
    FILE *f = fopen(FICHIER_CLASSES, "a");
    if (!f) {
        printf("\n❌ Erreur d'ouverture du fichier classes.\n");
        return;
    }

    Classes c;
    char buffer[100];

    printf("\nCode de la classe : ");
    fgets(buffer, sizeof(buffer), stdin);
    c.code = atoi(buffer);

    // Lecture du nom
    printf("Nom de la classe : ");
    fgets(c.nom, sizeof(c.nom), stdin);
    c.nom[strcspn(c.nom, "\n")] = '\0';  // Retirer le newline

    // Lecture du niveau
    printf("Niveau de la classe (Licence ou Master) : ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // Retirer le newline

    if (strcmp(buffer, "Licence") == 0) {
        c.niveau = Licence;
    } else if (strcmp(buffer, "Master") == 0) {
        c.niveau = Master;
    } else {
        printf("\n❌ Niveau invalide. Classe non ajoutée.\n");
        fclose(f);
        return;
    }

    FILE *fic_cls = fopen(FICHIER_CLASSES, "r");
    if (!fic_cls) {
        printf("\n❌ Erreur d'ouverture du fichier\n");
        fclose(f);
        return;
    }

    int code, verif = 0; 
    char ligne[256];


    fgets(ligne, sizeof(ligne), fic_cls);
    while (fgets(ligne, sizeof(ligne), fic_cls))
    {
        if ((sscanf(ligne, "%d,", &code) == 1) && (code == c.code))
        {
            verif = 1;
        }
        
    }
    fclose(fic_cls);

    if (verif == 1)
    {
        printf("Code deja pris.\nRecommencez en donnant un code plus grand\n");
        fclose(f);
        return;
    }
    
    

    // Écriture dans le fichier
    fprintf(f, "%d,%s,%s\n", c.code, c.nom, buffer);  // stocke le niveau en texte pour lisibilité
    fclose(f);
    printf("Classe ajoutée avec succès.\n");
}

void afficherClasses() {
    FILE *f = fopen(FICHIER_CLASSES, "r");
    if (!f) {
        printf("\n❌ Aucune classe enregistrée.\n");
        return;
    }

    Classes c;
    char ligne[256];
    fgets(ligne, sizeof(ligne), f);
    
    printf("\n----- LISTE DES CLASSES ENREGISTREES -----\n");
    printf("Code      | Nom            | Niveau\n");
    printf("------------------------------------------\n");
    
    int count = 0;
    while (fgets(ligne, sizeof(ligne), f)) {
        char niveautxt[20];
        sscanf(ligne, "%d,%[^,],%[^\n]", &c.code, c.nom, niveautxt);
        printf("%-10d | %-14s | %-15s\n", c.code, c.nom, niveautxt);
        count++;
    }
    
    printf("------------------------------------------\n");
    printf("Total: %d\n", count);
    
    fclose(f);
}

void rechercherClasse() {
    FILE *f = fopen(FICHIER_CLASSES, "r");
    if (!f) {
        printf("\n❌ Fichier introuvable.\n");
        return;
    }

    int codeRecherche;
    printf("\nCode de la classe : ");
    scanf("%d", &codeRecherche);
    getchar();

    Classes c;
    char niveautxt[15];
    char ligne[256];
    int trouve = 0;
    fgets(ligne, sizeof(ligne), f);

    while (fgets(ligne, sizeof(ligne), f)) {
        sscanf(ligne, "%d,%[^,],%[^\n]", &c.code, c.nom, niveautxt);
        if (c.code == codeRecherche) {
            printf("\n----- CLASSE TROUVEE -----\n");
            printf("Code  : %d\n", c.code);
            printf("Nom   : %s\n", c.nom);
            printf("Niveau: %s\n", niveautxt);
            printf("----------------------------\n");
            trouve = 1;
            break;
        }
    }
    if (!trouve) printf("\n❌ Classe non trouvée.\n");

    fclose(f);
}

void modifierClasse() {
    FILE *f = fopen(FICHIER_CLASSES, "r");
    if (!f) {
        printf("\n❌ Fichier introuvable.\n");
        return;
    }

    Classes liste[100];
    int n = 0;
    char ligne[100];

    

    while (fgets(ligne, sizeof(ligne), f)) {
    char niveauTxt[20];
    sscanf(ligne, "%d,%[^,],%[^\n]", &liste[n].code, liste[n].nom, niveauTxt);
    liste[n].niveau = (strcmp(niveauTxt, "Licence") == 0) ? Licence :
                      (strcmp(niveauTxt, "Master") == 0) ? Master : Licence; // par défaut
                      n++;
    }
    fclose(f);

    int codeModifie;
    char buffer[100];
    // Lecture du code (int)
    printf("Code de la classe a modifier : ");
    fgets(buffer, sizeof(buffer), stdin);
    codeModifie = atoi(buffer);

    int trouve = 0;
    for (int i = 0; i < n; i++) {
        if (liste[i].code == codeModifie) {
            printf("Nouveau nom de la classe (%s) : ", liste[i].nom);
            fgets(liste[i].nom, sizeof(liste[i].nom), stdin);
            liste[i].nom[strcspn(liste[i].nom, "\n")] = '\0';  // Retirer le newline

            printf("Niveau de la classe (Licence ou Master) : ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';  // Retirer le newline

            if (strcmp(buffer, "Licence") == 0) {
                liste[i].niveau = Licence;
            } else if (strcmp(buffer, "Master") == 0) {
                liste[i].niveau = Master;
            } else {
                printf("Niveau invalide. Classe non modifiee.\n");
                return;
            }

            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf(" Classe non trouvee.\n");
        return;
    }

    f = fopen(FICHIER_CLASSES, "w");
    fprintf(f, "Code,Nom,Niveau\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,%s,%s\n",
        liste[i].code,
        liste[i].nom,
        liste[i].niveau == Licence ? "Licence" : "Master");

    }
    fclose(f);
    printf(" Classe modifiée avec succes.\n");
}

void supprimerClasse() {
    FILE *f = fopen(FICHIER_CLASSES, "r");
    if (!f) {
        printf("❌ Fichier introuvable.\n");
        return;
    }

    Classes liste[1000];
    int n = 0;
    char ligne[256], copie[256];


    fgets(copie, sizeof(copie), f);
    while (fgets(ligne, sizeof(ligne), f)) {
        char niveauTxt[20];
        sscanf(ligne, "%d,%[^,],%[^\n]", &liste[n].code, liste[n].nom, niveauTxt);
        liste[n].niveau = (strcmp(niveauTxt, "Licence") == 0) ? Licence :
                          (strcmp(niveauTxt, "Master") == 0) ? Master : Licence;
        n++;
    }
    fclose(f);

    int codeSupp;
    printf("\nEntrer le code de la classe à supprimer : ");
    scanf("%d", &codeSupp);
    getchar();

    // Afficher la classe à supprimer et demander confirmation
    int trouve = 0;
    for (int i = 0; i < n; i++) {
        if (liste[i].code == codeSupp) {
            printf("\n----- CLASSE A SUPPRIMER -----\n");
            printf("Code  : %d\n", liste[i].code);
            printf("Nom   : %s\n", liste[i].nom);
            printf("Niveau: %s\n", (liste[i].niveau == Licence) ? "Licence" : "Master");
            printf("------------------------------\n");
            
            int confirmation;
            printf("\nConfirmez-vous la suppression ? (1 pour confirmer / 0 pour annuler) : ");
            scanf("%d", &confirmation);
            
            if (confirmation != 1) {
                printf("❌ Suppression annulée.\n");
                return;
            }
            
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        printf("\n❌ Classe non trouvée.\n");
        return;
    }

    FILE *fout = fopen(FICHIER_CLASSES, "w");
    if (!fout) {
        printf("❌ Erreur lors de l'ouverture du fichier en écriture.\n");
        return;
    }

    fputs(copie, fout);

    for (int i = 0; i < n; i++) {
        if (liste[i].code != codeSupp) {
            fprintf(fout, "%d,%s,%s\n",
                    liste[i].code,
                    liste[i].nom,
                    (liste[i].niveau == Licence) ? "Licence" : "Master");
        }
    }

    fclose(fout);
    printf("\n✓ Classe supprimée avec succès.\n");
}
