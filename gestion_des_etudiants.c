#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_des_etudiants.h"



typedef enum {Licence, Master} Niveau;

typedef struct {
    char nom[10];
    int code;
    Niveau niveau;
} Classes;


typedef struct 
{
    char libelle[35];
    short coefficient;
    Classes classes[];
} Matieres;


typedef struct 
{
    int id_etudiant;
    char nom[30];
    char prenom[20];
    char email[30];
    char date_naissance[15];
    Classes classe;
}  Etudiants;


typedef struct 
{
    Matieres matieres;
    Classes classes;
} Mat_Classe;

typedef struct
{
    Matieres matiere;
    Etudiants etudiant;
    float noteCC;
    float noteDS;
} Notes;


Etudiants etudiants[10000];
Classes c;


void ajout_etudiant_csv(const char *nom_fichier, Etudiants *etudiants) {
    FILE *fic = fopen(nom_fichier, "a");
    if (fic == NULL) {
        printf("\n❌ Erreur d'allocation du fichier.\n");
        exit(1);
    }
    fprintf(fic, "\n%d,%s,%s,%s,%s,%d\n", etudiants->id_etudiant, etudiants->nom, etudiants->prenom, etudiants->email, etudiants->date_naissance, etudiants->classe.code);
    fclose(fic);
}

void ajout_etudiant() {
    Etudiants etudiants;
    int dernier_id = 0;

    FILE *f = fopen("etudiants.csv", "r");
    if (f != NULL) {
        char ligne[256];
        fgets(ligne, 256, f);

        while (fgets(ligne, 256, f) != NULL)
        {
            int id_tmp;
            if (sscanf(ligne, "%d,", &id_tmp) == 1)
            {
                dernier_id = id_tmp;
            }
            
        }
        

    fclose(f);
    }

    etudiants.id_etudiant = dernier_id + 1;
    getchar();
    printf("\n--- Ajout d'un nouvel étudiant ---\n");
    printf("Nom : ");
    scanf(" %[^\n]", etudiants.nom);
    printf("Prénom : ");
    scanf(" %[^\n]", etudiants.prenom);
    printf("Email : ");
    scanf(" %[^\n]", etudiants.email);
    printf("Date de naissance (YYYY-MM-DD) : ");
    scanf(" %[^\n]", etudiants.date_naissance);
    


    int code;
    char classe[10], ligne[256];
    int trouve = 0;

    do {
        FILE *fic_cls = fopen("classes.csv", "r");
        if (!fic_cls) {
            printf("❌ Erreur d'ouverture du fichier des classes.\n");
            return;
        }

        printf("Liste des classes disponibles :\n");
        printf("Code      | Nom\n");
        printf("-----------------------\n");

        if (fgets(ligne, sizeof(ligne), fic_cls)) { /* ignorer l'entête */ }

        while (fgets(ligne, sizeof(ligne), fic_cls)) {
            if (sscanf(ligne, "%d,%9[^,]", &c.code, c.nom) == 2) {
                printf("%-10d|%-10s\n", c.code, c.nom);
            }
        }

        fclose(fic_cls);

        printf("Choisissez un code pour la classe de l'étudiant (ou 0 pour annuler) : ");
        scanf("%d", &code);
        getchar();

        if (code == 0) return;

        FILE *fic_cls2 = fopen("classes.csv", "r");
        if (!fic_cls2) {
            printf("❌ Erreur d'ouverture du fichier des classes.\n");
            return;
        }

        if (fgets(ligne, sizeof(ligne), fic_cls2)) { /* ignorer l'entête */ }

        while (fgets(ligne, sizeof(ligne), fic_cls2)) {
            if (sscanf(ligne, "%d,%9[^,]", &c.code, c.nom) == 2 && c.code == code) {
                trouve = 1;
                break;
            }
        }

        fclose(fic_cls2);
        if (!trouve)
            printf("❌ Classe non trouvée. Réessayez.\n");

    } while (!trouve);

    etudiants.classe.code = code;
    ajout_etudiant_csv("etudiants.csv", &etudiants);
    printf("\n✓ Étudiant ajouté avec succès! (ID: %d)\n", etudiants.id_etudiant);
}





void modif_etudiants_csv(const char *nom_fichier, int id_modif, Etudiants *etudiants) {
    FILE *fic = fopen(nom_fichier, "r");
    FILE *f = fopen("temp.csv", "w");

    char ligne[256];
    fgets(ligne, sizeof(ligne), fic);
    fputs(ligne,f);
    int id;

    while (fgets(ligne, 256, fic))
    {
        if ((sscanf(ligne, "%d,", &id)==1) && (id != id_modif)) 
        fputs(ligne, f);

        if ((sscanf(ligne, "%d,", &id) == 1) && (id == id_modif))
        fprintf(f, "\n%d,%s,%s,%s,%s,%d\n", etudiants->id_etudiant, etudiants->nom, etudiants->prenom, etudiants->email, etudiants->date_naissance, etudiants->classe.code);
    }
    
    
    fclose(fic);
    fclose(f);


    remove(nom_fichier);
    rename("temp.csv", nom_fichier);
}

void modif_etudiant() {
    printf("\n=== MODIFICATION D'UN ÉTUDIANT ===\n");
    printf("ID de l'étudiant à modifier : ");
    int id_modif, id, trouve = 0;
    scanf("%d", &id_modif);

    do {
        FILE *fic = fopen("etudiants.csv", "r");
        if (fic == NULL) exit(1);
        char ligne[256];
        fgets(ligne, sizeof(ligne), fic); // Sauter l'en-tête

        trouve = 0;
        while (fgets(ligne, sizeof(ligne), fic)) {
            if (sscanf(ligne, "%d,", &id) == 1 && id == id_modif) {
                trouve = 1;
                break;
            }
        }
        fclose(fic);

        if (!trouve) {
            printf("\n❌ ID non trouvé. Entrez un autre ID (0 pour annuler): ");
            scanf("%d", &id_modif);
            if (id_modif == 0) {
                printf("Modification annulée.\n");
                return;
            }
        }
    } while (!trouve);

    Etudiants etudiants_modif;
    etudiants_modif.id_etudiant = id_modif;
    getchar();
    printf("\n--- Modification de l'étudiant (ID: %d) ---\n", id_modif);
    printf("Nouveau nom : ");
    scanf(" %[^\n]", etudiants_modif.nom);
    printf("Nouveau prénom : ");
    scanf(" %[^\n]", etudiants_modif.prenom);
    printf("Nouvel email : ");
    scanf(" %[^\n]", etudiants_modif.email);
    printf("Nouvelle date de naissance (YYYY-MM-DD) : ");
    scanf(" %[^\n]", etudiants_modif.date_naissance);
    int code;
    char classe[10], ligne[256];
    trouve = 0;

    do {
        FILE *fic_cls = fopen("classes.csv", "r");
        if (!fic_cls) {
            printf("❌ Erreur d'ouverture du fichier des classes.\n");
            return;
        }

        printf("Liste des classes disponibles :\n");
        printf("Code      | Nom\n");
        printf("-----------------------\n");

        if (fgets(ligne, sizeof(ligne), fic_cls)) { /* ignorer l'entête */ }

        while (fgets(ligne, sizeof(ligne), fic_cls)) {
            if (sscanf(ligne, "%d,%9[^,]", &c.code, c.nom) == 2) {
                printf("%-10d|%-10s\n", c.code, c.nom);
            }
        }

        fclose(fic_cls);

        printf("Choisissez un code pour la classe de l'étudiant (ou 0 pour annuler) : ");
        scanf("%d", &code);
        getchar();

        if (code == 0) return;

        FILE *fic_cls2 = fopen("classes.csv", "r");
        if (!fic_cls2) {
            printf("❌ Erreur d'ouverture du fichier des classes.\n");
            return;
        }

        if (fgets(ligne, sizeof(ligne), fic_cls2)) { /* ignorer l'entête */ }

        while (fgets(ligne, sizeof(ligne), fic_cls2)) {
            if (sscanf(ligne, "%d,%9[^,]", &c.code, c.nom) == 2 && c.code == code) {
                trouve = 1;
                break;
            }
        }

        fclose(fic_cls2);
        if (!trouve)
            printf("❌ Classe non trouvée. Réessayez.\n");

    } while (!trouve);

    etudiants_modif.classe.code = code;

    modif_etudiants_csv("etudiants.csv", id_modif, &etudiants_modif);
    printf("\n✓ Étudiant modifié avec succès!\n");
}



void rechercher_etudiant() {
    Etudiants *etudiants;

    etudiants = malloc(sizeof(Etudiants));
if (etudiants == NULL) {
    fprintf(stderr, "Erreur d'allocation de memoire\n");
    exit(1);
}

    int val = 1;
    while (val == 1) {
        int search;
        printf("\n=== RECHERCHE D'UN ÉTUDIANT ===\n");
        printf("ID de l'étudiant à rechercher : ");
        scanf("%d", &search);

        int retour = 0;
        FILE *fic =  fopen("etudiants.csv", "r");
        if (fic == NULL) exit(1);


        FILE *f = fopen("classes.csv", "r");
        if (!f) exit(1);
        
        char ligne[256], chaine[256];
        int code;

        fgets(ligne, 256, fic);
        fgets(chaine, 256, f);

        while (fgets(ligne, sizeof(ligne), fic))
        {
            if((sscanf(ligne,"%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &etudiants->id_etudiant, etudiants->nom, etudiants->prenom, etudiants->email, etudiants->date_naissance, &etudiants->classe.code) == 6) && (etudiants->id_etudiant == search)) {
                while (fgets(chaine, 256, f)) {
                    if ((sscanf(chaine, "%d,%9[^,]", &code, etudiants->classe.nom)) == 2 && (code == etudiants->classe.code))
                    {
                        printf("Voici l'étudiant recherché : \n");
                        printf("ID : %d\n", etudiants->id_etudiant);
                        printf("Prénom : %s\n", etudiants->prenom);
                        printf("Nom : %s\n", etudiants->nom);
                        printf("Classe : %s\n", etudiants->classe.nom);
                        retour = 1;
                        break;
                    }
                }
            }
        }
        fclose(fic);

        if (!retour) {
            printf("\n❌ Étudiant non trouvé.\n");
            printf("Voulez vous rechercher encore? (1: oui / 0: retour menu): ");
            scanf("%d", &val);
        } else {
            val = 0;
        }
    }
}



void suppr_etudiants_csv(const char *nom_fichier, int id_suppr) {
    FILE *fic = fopen(nom_fichier, "r");
    if (!fic) exit(1);
    FILE *f = fopen("temp.csv", "w");
    if (!f) exit(1);

    char ligne[256];
    fgets(ligne, sizeof(ligne), fic);
    fputs(ligne,f);
    int id;

    while (fgets(ligne, 256, fic))
    {
        if ((sscanf(ligne, "%d,", &id)==1) && (id != id_suppr)) 
        fputs(ligne, f);

    }
       
    fclose(fic);
    fclose(f);


    remove(nom_fichier);
    rename("temp.csv", nom_fichier);
}
//4
void suppr_etudiant() {
    Etudiants *etudiants;
    etudiants = malloc(sizeof(Etudiants));
if (etudiants == NULL) {
    fprintf(stderr, "Erreur d'allocation de mémoire\n");
    exit(1);
}


    printf("Bienvenu dans le menu de suppression.\n");
    printf("Renseigner l'ID de l'étudiant à supprimer : ");
    int id_suppr, id, trouve = 0;
    scanf("%d", &id_suppr);

    do {
        FILE *fic = fopen("etudiants.csv", "r");
        if (fic == NULL) exit(1);

        FILE *f = fopen("classes.csv", "r");
        if (!f) exit(1);

        
        char ligne[256], chaine[256];
        int code;
        fgets(ligne, sizeof(ligne), fic); // Sauter l'en-tête
        fgets(chaine, sizeof(chaine), f); // Sauter l'en-tête

        trouve = 0;
        while (fgets(ligne, sizeof(ligne), fic)) {
            if ((sscanf(ligne,"%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &etudiants->id_etudiant, etudiants->nom, etudiants->prenom, etudiants->email, etudiants->date_naissance, &etudiants->classe.code) == 6) && (etudiants->id_etudiant == id_suppr)) {
                trouve = 1;
                if ((sscanf(chaine, "%d,%9[^,]", &code, etudiants->classe.nom)) == 2 && (code == etudiants->classe.code))
                {
                    printf("Voici l'étudiant à supprimer : \n");
                    printf("ID : %d\n", etudiants->id_etudiant);
                    printf("Prénom : %s\n", etudiants->prenom);
                    printf("Nom : %s\n", etudiants->nom);
                    printf("Classe : %s\n", etudiants->classe.nom);
                    break;
                }
                
               
            }
        }
        fclose(fic);
        free(etudiants);



        if (!trouve) {
            printf("❌ L'ID fourni n'existe pas dans la base, veuillez fournir un bon id ou taper 0 pour annuler la suppression\n");
            scanf("%d", &id_suppr);
            if (id_suppr == 0) {
                return;
            }
        }
    } while (!trouve);

    printf("Voulez vous confirmer la suppression ? (1) : ");
    int reponse;
    scanf(" %d", &reponse);
    if (reponse == 1)
    {
        suppr_etudiants_csv("etudiants.csv", id_suppr);
        printf("✓ L'étudiant a bien été supprimé\n");
    }
    else 
    {
        printf("Suppression annulée\n");
    }
}




void affiche_classe(int code) {

    Etudiants *etudiants;
     etudiants = malloc(sizeof(Etudiants));
if (etudiants == NULL) {
    fprintf(stderr, "Erreur d'allocation de mémoire\n");
    exit(1);
}



    char nom_classe[10];
    int code_classe;
    FILE *fic_etud = fopen("etudiants.csv","r");
    if (!fic_etud) exit(1);

    char ligne[256];
    fgets(ligne, 256, fic_etud);

    while (fgets(ligne, sizeof(ligne), fic_etud))
    {
        if ((sscanf(ligne, "%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &etudiants->id_etudiant, etudiants->nom, etudiants->prenom, etudiants->email, etudiants->date_naissance, &etudiants->classe.code)) == 6 && (etudiants->classe.code == code))
        {
            printf("ID            |Pr\u00e9nom              |Nom                           \n");
            printf("%-14d|%-20s|%-30s\n",  etudiants->id_etudiant, etudiants->prenom, etudiants->nom);
        }
        
    }

    fclose(fic_etud);
    free (etudiants);
    


}

void affiche_tous() {
        Etudiants *etudiants;
     etudiants = malloc(sizeof(Etudiants));
if (etudiants == NULL) {
    fprintf(stderr, "Erreur d'allocation de mémoire\n");
    exit(1);
}


    char classe[10];
    int code;


    FILE *fic_etud = fopen("etudiants.csv","r");
    if (!fic_etud) exit(1);

    FILE *fic_cls = fopen("classes.csv", "r");
    if (!fic_cls) exit(1);

    char ligne[256], chaine[256];


    fgets(ligne, 256, fic_etud);
    fgets(chaine, 256, fic_cls);

    printf("ID            |Prénom              |Nom                           |Classe    \n");
    while (fgets(ligne, sizeof(ligne), fic_etud))
    {
        if (sscanf(ligne, "%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &etudiants->id_etudiant, etudiants->nom, etudiants->prenom, etudiants->email, etudiants->date_naissance, &etudiants->classe.code) == 6)
        {

            rewind(fic_cls);
            while (fgets(chaine, 256, fic_cls))
            {
                if ( ((sscanf(chaine, "%d,%9[^,]", &code, classe)) == 2 && (code == etudiants->classe.code)))
                {
                    printf("%-14d|%-20s|%-30s|%-10s\n",  etudiants->id_etudiant, etudiants->prenom, etudiants->nom, classe);
                }
                
            }
            
        }
        
    }

    fclose(fic_etud);
    free (etudiants);
}

//5
void affiche_etudiants() {
    short option;

    printf("1- Afficher les étudiants d'une classe\n");
    printf("2- Afficher tous les étudiants\n");
    do
    {
        printf("Choisir une option : ");
        scanf("%d", &option);
        getchar();

    } while (option < 0 || option > 2);


    if (option == 0)
    {
        return;
    }
    

    if (option == 2)
    {
        affiche_tous();
    }
    


    int i = 0;
    int code_classe[1000];
    char ligne[256], nom_classe[10];

    if (option == 1) {
        printf("Choisissez le code parmi les classes suivantes\n");
        FILE *fic = fopen("classes.csv", "r");
        if (!fic) exit(1);


        
        fgets(ligne, sizeof(ligne), fic);
        while (fgets(ligne, 256, fic))
        {
            sscanf(ligne, "%d,%9[^,]", &code_classe[i], nom_classe);
            printf("%10s :\t %3d\n", nom_classe, code_classe[i]);
            i++;
        }
        fclose(fic);


        int code, retourne = 0;
        printf("Code choisi : ");
        scanf("%d", &code);
        for ( i = 0; i < 1000 ; i++)
        {
            if (code_classe[i] = code) retourne = 1;
        }

        if (retourne == 0)
        {
            printf("❌ Ce code est introuvable. Relancez le programme\n");
        }

        else if (retourne == 1)
        {
            affiche_classe(code);
        }
    }

    

    
    
    

}




void gestion_des_etudiants () {
    printf("\n===== GESTION DES ÉTUDIANTS =====\n");
    short rep;
do {
   
    printf("1. Ajout\n");
    printf("2. Modification\n");
    printf("3. Recherche\n");
    printf("4. Suppression\n");
    printf("5. Affichage\n");
    printf("0. Aller vers le menu principal\n");
    printf("Choisissez un chiffre dans la liste des options précédents: \n");
    scanf("%d", &rep);
    getchar();

    switch(rep) {
        case 1: ajout_etudiant();
            break;
        case 2: modif_etudiant();
            break;
        case 3: rechercher_etudiant();
            break;
        case 4: suppr_etudiant();
            break;
        case 5: affiche_etudiants();
            break;
        case 0: printf("Retour au menu principal...\n");
            break;
        default : 
                printf("❌ L'option choisie n'est pas disponible\n");
    } }
    while (rep != 0);
    return;

}