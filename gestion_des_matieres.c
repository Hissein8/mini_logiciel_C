#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_des_matieres.h"

#define FICHIER_MATIERES "matieres.csv"


typedef enum {Licence, Master} Niveau;

typedef struct {
    int code;
    char nom[10];
    Niveau niveau;
} Classes;

typedef struct 
{
    int code;
    char libelle[35];
    short coefficient;
    Classes classes[];
} Matieres;


void ajouterMatiere();
void afficherMatieres();
void modifierMatiere();
void supprimerMatiere();
void associerMatiere();
void dissocierMatiere();
void listeAssociation();


void menuMatiere() {
    int choix;
    do {
        printf("\n===== GESTION DES MATIÈRES =====\n");
        printf("1. Ajouter une matière\n");
        printf("2. Afficher toutes les matières\n");
        printf("3. Modifier une matière\n");
        printf("4. Supprimer une matière\n");
        printf("5. Associer une matière à une classe\n");
        printf("6. Dissocier une matière d'une classe\n");
        printf("7. Lister les matières associées aux classes\n");
        printf("0. Retour\n");
        printf("Choix : ");
        scanf("%d", &choix);
        getchar();
        switch (choix) {
            case 1: ajouterMatiere(); break;
            case 2: afficherMatieres(); break;
            case 3: modifierMatiere(); break;
            case 4: supprimerMatiere(); break;
            case 5: associerMatiere(); break;
            case 6: dissocierMatiere(); break;
            case 7: listeAssociation(); break;
            case 0: printf("Retour au menu principal...\n"); break;
            default: printf("\n❌ Choix invalide.\n");
        }
    } while (choix != 0);
    return;
}

void gestion_des_matieres() {
    menuMatiere();
    return;
}


void ajouterMatiere() {
    int ref, coef;
    char nom[35];

    Matieres m;
    printf("\nChoisissez un code (entier) pour la matière : ");
    scanf("%d", &m.code);
    while (getchar() != '\n');  // vider le buffer stdin

    printf("Libellé : ");
    fgets(m.libelle, sizeof(m.libelle), stdin);
    m.libelle[strcspn(m.libelle, "\n")] = '\0';  // enlever le saut de ligne

    printf("Coefficient : ");
    scanf("%hd", &m.coefficient);
    while (getchar() != '\n');  // nettoyer

    FILE *f = fopen(FICHIER_MATIERES, "r+");
    if (!f) {
        printf("\n❌ Erreur d'ouverture du fichier.\n");
        return;
    }

    char ligne[256];
    int doublon = 0;

    // Lire l'en-tête
    if (fgets(ligne, sizeof(ligne), f)) {
        // Ne rien faire, juste sauter
    }

    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%d,%34[^,],%d", &ref, nom, &coef) == 3) {
            nom[strcspn(nom, "\n")] = '\0';  // Nettoyer newline
            if (ref == m.code) {
                printf("\n❌ Ce code est déjà pris. Veuillez utiliser un autre code.\n");
                doublon = 1;
                break;
            }
            if (strcmp(nom, m.libelle) == 0) {
                printf("\n❌ Ce libellé est déjà utilisé. Veuillez utiliser un libellé différent.\n");
                doublon = 1;
                break;
            }
        }
    }

    if (!doublon) {
        // Aller à la fin pour écrire
        fseek(f, 0, SEEK_END);
        fprintf(f, "\n%d,%s,%hd\n", m.code, m.libelle, m.coefficient);
        printf("\n✓ Matière ajoutée avec succès!\n");
    }

    fclose(f);
}

void afficherMatieres() {
    FILE *f = fopen(FICHIER_MATIERES, "r");
    if (!f) {
        printf("\n❌ Aucune matière enregistrée.\n");
        return;
    }

    char ligne[256];
    fgets(ligne, sizeof(ligne), f);

    Matieres m;
    printf("\n----- LISTE DES MATIERES ENREGISTREES -----\n");
    printf("Code      | Libelle                     | Coeff.\n");
    printf("--------------------------------------------------\n");
    
    int count = 0;
    while (fscanf(f, "%d,%34[^,],%hd\n", &m.code, m.libelle, &m.coefficient) != EOF) {
        m.libelle[strcspn(m.libelle, "\n")] = '\0';  // Nettoyer newline
        printf("%-10d | %-27s | %-5hd\n", m.code, m.libelle, m.coefficient);
        count++;
    }
    
    printf("--------------------------------------------------\n");
    printf("Total: %d\n", count);
    
    fclose(f);
}

void modifierMatiere() {
    int code;
    printf("\nEntrez le code de la matière à modifier : ");
    scanf("%d", &code);
    while (getchar() != '\n');  // Purge du buffer

    FILE *f = fopen(FICHIER_MATIERES, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!f || !temp) {
        printf("\n❌ Erreur lors de l'ouverture du fichier.\n");
        if (f) fclose(f);
        if (temp) fclose(temp);
        return;
    }

    char ligne[256];
    Matieres m;
    int found = 0;

    // Copie de l’en-tête
    if (fgets(ligne, sizeof(ligne), f)) {
        fputs(ligne, temp);
    }

    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%d,%34[^,],%hd", &m.code, m.libelle, &m.coefficient) == 3) {
            m.libelle[strcspn(m.libelle, "\n")] = '\0';  // Nettoyer newline
            if (code == m.code) {
                printf("Nouveau libelle : ");
                fgets(m.libelle, sizeof(m.libelle), stdin);
                m.libelle[strcspn(m.libelle, "\n")] = '\0';

                printf("Nouveau coefficient : ");
                scanf("%hd", &m.coefficient);
                while (getchar() != '\n');

                found = 1;
            }
            fprintf(temp, "%d,%s,%hd\n", m.code, m.libelle, m.coefficient);
        } else {
            // Ligne mal formatée, on la conserve telle quelle
            fputs(ligne, temp);
        }
    }

    fclose(f);
    fclose(temp);

    if (remove(FICHIER_MATIERES) != 0 || rename("temp.csv", FICHIER_MATIERES) != 0) {
        printf("Erreur lors de la sauvegarde des modifications.\n");
        return;
    }

    if (found)
        printf("\n✓ Matière modifiée avec succès!\n");
    else
        printf("\n❌ Matière introuvable.\n");
}

void supprimerMatiere() {
    int code;
    printf("\nEntrez le code de la matière à supprimer : ");
    scanf("%d", &code);


    FILE *f = fopen(FICHIER_MATIERES, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!f || !temp) {
        printf("\n❌ Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    char ligne[256];
    if (fgets(ligne, sizeof(ligne), f)) {
    fputs(ligne, temp);  // copie l’en-tête
    }

    Matieres m;
    int found = 0;
    while (fscanf(f, "%d,%[^,],%hd\n", &m.code, m.libelle, &m.coefficient) != EOF) {
        m.libelle[strcspn(m.libelle, "\n")] = '\0';  // Nettoyer newline
        if (code != m.code) {
            fprintf(temp, "\n%d,%s,%hd\n", m.code, m.libelle, m.coefficient);
        } else {
            found = 1;
        }
    }

    fclose(f);
    fclose(temp);
    remove(FICHIER_MATIERES);
    rename("temp.csv", FICHIER_MATIERES);

    if (found) printf("\n✓ Matière supprimée avec succès!\n");
    else printf("\n❌ Matière introuvable.\n");
}

void associerMatiere() {
    int ref, coef;
    Matieres m;
    char nom[35];
    char ligne[256];
    int trouve = 0;

    // Selection de la matiere
    do {
        printf("\nEntrer le code de la matière à associer (ou 0 pour quitter) : ");
        scanf("%d", &ref);
        while (getchar() != '\n');

        if (ref == 0) return;

        FILE *fic_mat = fopen(FICHIER_MATIERES, "r");
        if (!fic_mat) {
            printf("\n❌ Erreur d'ouverture du fichier des matières.\n");
            return;
        }

        if (fgets(ligne, sizeof(ligne), fic_mat)) { /* ignorer l'entete */ }

        while (fgets(ligne, sizeof(ligne), fic_mat)) {
            if (sscanf(ligne, "%d,%34[^,],%d", &m.code, m.libelle, &m.coefficient) == 3 && m.code == ref) {
                m.libelle[strcspn(m.libelle, "\n")] = '\0';  // Nettoyer newline
                coef = m.coefficient;
                strcpy(nom, m.libelle);
                trouve = 1;
                break;
            }
        }

        fclose(fic_mat);
        if (!trouve)
            printf("\n❌ Matière non trouvée. Réessayez.\n");

    } while (!trouve);

    // Selection de la classe
    int code;
    Classes c;
    char classe[10];
    trouve = 0;

    do {
        FILE *fic_cls = fopen("classes.csv", "r");
        if (!fic_cls) {
            printf("\n❌ Erreur d'ouverture du fichier des classes.\n");
            return;
        }

        printf("\nCode      | Nom\n");
        printf("-----------------------\n");

        if (fgets(ligne, sizeof(ligne), fic_cls)) { /* ignorer l'entete */ }

        while (fgets(ligne, sizeof(ligne), fic_cls)) {
            if (sscanf(ligne, "%d,%9[^,]", &c.code, c.nom) == 2) {
                printf("%-10d|%-10s\n", c.code, c.nom);
            }
        }

        fclose(fic_cls);

        printf("\nChoisissez un code pour l'association (ou 0 pour annuler) : ");
        scanf("%d", &code);
        while (getchar() != '\n');

        if (code == 0) return;

        FILE *fic_cls2 = fopen("classes.csv", "r");
        if (!fic_cls2) {
            printf("\n❌ Erreur d'ouverture du fichier des classes.\n");
            return;
        }

        if (fgets(ligne, sizeof(ligne), fic_cls2)) { /* ignorer l'entete */ }

        while (fgets(ligne, sizeof(ligne), fic_cls2)) {
            if (sscanf(ligne, "%d,%9[^,]", &c.code, c.nom) == 2 && c.code == code) {                c.nom[strcspn(c.nom, "\n")] = '\0';  // Nettoyer newline                strcpy(classe, c.nom);
                trouve = 1;
                break;
            }
        }

        fclose(fic_cls2);
        if (!trouve)
            printf("\n❌ Classe non trouvée. Réessayez.\n");

    } while (!trouve);

    // Confirmation
    int confirm;
    printf("\nConfirmez l'association de [%s] à la classe [%s] ? Tapez 0 pour valider : ", nom, classe);
    scanf("%d", &confirm);
    while (getchar() != '\n');

    if (confirm != 0) {
        printf("\n❌ Opération annulée.\n");
        return;
    }

    // Enregistrement de l'association
    FILE *fic = fopen("matieres_classes.csv", "a");
    if (!fic) {
        printf("\n❌ Erreur lors de l'ouverture du fichier d'association.\n");
        return;
    }

    fprintf(fic, "\n%d,%s,%d,%s\n", code, classe, ref, nom);
    fclose(fic);

    printf("\n✓ Association réussie!\n");
}

void dissocierMatiere() {
    int ref, coef, trouve = 0;
    Matieres m;
    char nom[30], ligne[256];

    // Recherche de la matiere
    do {
        printf("\nEntrer le code de la matière à dissocier (ou 0 pour quitter) : ");
        scanf("%d", &ref);
        if (ref == 0) return;
        while (getchar() != '\n');

        FILE *fic_mat = fopen(FICHIER_MATIERES, "r");
        if (!fic_mat) {
            printf("\n❌ Erreur ouverture fichier matières\n");
            return;
        }

        if (fgets(ligne, sizeof(ligne), fic_mat)) { /* entete ignoree */ }

        while (fgets(ligne, sizeof(ligne), fic_mat)) {
            if (sscanf(ligne, "%d,%29[^,],%d", &m.code, m.libelle, &m.coefficient) == 3 && m.code == ref) {
                m.libelle[strcspn(m.libelle, "\n")] = '\0';  // Nettoyer newline
                strcpy(nom, m.libelle);
                coef = m.coefficient;
                trouve = 1;
                break;
            }
        }

        fclose(fic_mat);
        if (!trouve)
            printf("\n❌ Matière introuvable. Veuillez réessayer.\n");

    } while (!trouve);

    // Recherche de la classe
    int code;
    Classes c;
    char classe[10];
    trouve = 0;

    do {
        FILE *fic_cls = fopen("classes.csv", "r");
        if (!fic_cls) {
            printf("\n❌ Erreur ouverture fichier classes\n");
            return;
        }

        printf("\nCode      | Nom\n");
        printf("--------------------\n");

        if (fgets(ligne, sizeof(ligne), fic_cls)) { /* entete ignoree */ }

        while (fgets(ligne, sizeof(ligne), fic_cls)) {
            if (sscanf(ligne, "%d,%9[^,]", &c.code, c.nom) == 2)
                printf("%-10d|%-10s\n", c.code, c.nom);
        }

        fclose(fic_cls);

        printf("\nChoisissez un code pour la dissociation (ou 0 pour quitter) : ");
        scanf("%d", &code);
        if (code == 0) return;
        while (getchar() != '\n');

        FILE *fic_cls2 = fopen("classes.csv", "r");
        if (!fic_cls2) {
            printf("\n❌ Erreur ouverture fichier classes\n");
            return;
        }

        if (fgets(ligne, sizeof(ligne), fic_cls2)) { /* entete ignoree */ }

        while (fgets(ligne, sizeof(ligne), fic_cls2)) {
            if (sscanf(ligne, "%d,%9[^,]", &c.code, c.nom) == 2 && c.code == code) {
                c.nom[strcspn(c.nom, "\n")] = '\0';  // Nettoyer newline
                strcpy(classe, c.nom);
                trouve = 1;
                break;
            }
        }

        fclose(fic_cls2);
        if (!trouve)
            printf("\n❌ Code de classe introuvable. Réessayer.\n");

    } while (!trouve);

    // Confirmation
    int confirm;
    printf("\nConfirmez la dissociation de %s et la classe %s ? Tapez 0 pour valider : ", nom, classe);
    scanf("%d", &confirm);
    while (getchar() != '\n');
    if (confirm != 0) {
        printf("\n❌ Opération annulée.\n");
        return;
    }

    // Suppression de l'association
    int success = 0;
    FILE *fic = fopen("matieres_classes.csv", "r");
    FILE *tmp = fopen("temp.csv", "w");

    if (!fic || !tmp) {
        printf("\n❌ Erreur ouverture fichiers\n");
        return;
    }

    if (fgets(ligne, sizeof(ligne), fic)) fputs(ligne, tmp);

    while (fgets(ligne, sizeof(ligne), fic)) {
        if (sscanf(ligne, "%d,%[^,],%d,%[^\n]", &c.code, c.nom, &m.code, m.libelle) == 4
            && (c.code != code || m.code != ref)) {
            fputs(ligne, tmp);
        } else {
            success = 1;
        }
    }

    fclose(fic);
    fclose(tmp);

    remove("matieres_classes.csv");
    rename("temp.csv", "matieres_classes.csv");

    if (success)
        printf("\n✓ Dissociation réussie!\n");
    else
        printf("\n❌ Échec dissociation ou éléments non associés\n");
}

void listeAssociation() {
    Classes c;
    Matieres m;
    printf("\n----- ASSOCIATIONS MATIERES-CLASSES -----\n");
    printf("Code Class | Classe          | Ref. Mat.  | Matière\n");
    printf("----------------------------------------------------\n");

    FILE *fic = fopen("matieres_classes.csv", "r");
    if (!fic) {
        printf("❌ Erreur d'ouverture du fichier\n");
        printf("---------------------------------------------\n");
        return;
    }

    char ligne[256];
    fgets(ligne, sizeof(ligne), fic);  // Ignorer l'en-tête

    int count = 0;
    while (fgets(ligne, sizeof(ligne), fic)) {
        if (sscanf(ligne, "%d,%[^,],%d,%[^\n]", &c.code, c.nom, &m.code, m.libelle) == 4) {
            printf("%-10d | %-15s | %-10d | %-7s\n", c.code, c.nom, m.code, m.libelle);
            count++;
        }
    }
    
    printf("----------------------------------------------------\n");
    printf("Total: %d\n", count);

    fclose(fic);
}
