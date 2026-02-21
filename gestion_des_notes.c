#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_des_notes.h"


typedef enum {Licence, Master} Niveau;

typedef struct {
    int code;
    char nom[30];
    Niveau niveau;
} Classes;


typedef struct 
{
    int ref;
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



void consultation();
void modification();
void suppression();
void ajout();


void ajout_note_etudiant();
void ajout_notes_etudiant();
void ajout_notes_classe();
void suppression_note_etudiant();
void suppression_notes_etudiant();
void suppression_notes_classe();
void consultation_note_etudiant();
void consultation_notes_etudiant();
void consultation_notes_classe();
void modification_note_etudiant();
void modification_notes_etudiant();
void modification_notes_classe();

void gestion_des_notes(void) {
    printf("\n===== GESTION DES NOTES =====\n");
    short rep;
    do {
        printf("1. Ajout\n");
        printf("2. Modification\n");
        printf("3. Consultation\n");
        printf("4. Suppression\n");
        printf("0. Aller vers le menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &rep);
        getchar();

    switch(rep) {
        case 1: ajout();
            break;
        case 2: modification();
            break;
        case 3: consultation();
            break;
        case 4: suppression();
            break;
        case 0: printf("Retour au menu principal...\n");
            break;
        default:
            printf("Option invalide\n");
    }
    } while (rep != 0);
    return;
}




//1
void ajout() {
    short choix;
    printf("\n=== MENU AJOUT ===\n");
    printf("1. Ajout des notes d'un étudiant dans une matière\n");
    printf("2. Ajout des notes d'un étudiant dans toutes ses matières\n");
    printf("3. Ajout des notes d'une classe dans une matière\n");
    printf("Menu précédent\n");
    printf("Votre choix : ");
    scanf("%d", &choix);
    getchar();
    while (choix < 0 || choix > 3) {
        printf("Veuillez entrer un numero valide : ");
        scanf("%d", &choix);
        getchar();
    }
    switch (choix)
    {
    case 1:
        ajout_note_etudiant();
        break;
    case 2:
        ajout_notes_etudiant();
        break;
    case 3:
        ajout_notes_classe();
        break;
    default:
        break;
    }
}

void ajout_note_etudiant() {

    int id, rep, ref, coef, verif, retour = 0;
    char ligne[256], chaine[256];
    char libelle[35], nom [30], prenom[20];
    do {
        printf("Renseigner l'id de l'étudiant voulu (ou 0 pour un retour): ");
        scanf("%d", &id);
        if (id == 0) return;
        FILE *fic = fopen("etudiants.csv", "r");
        if(!fic) exit(1);
        fgets(ligne, 256, fic);

        while (fgets(ligne, sizeof(ligne), fic))
        {
            int id_lu, code_classe;
            char nom_lu[30], prenom_lu[20], email[30], date_naissance[15];
            if (sscanf(ligne, "%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &id_lu, nom_lu, prenom_lu, email, date_naissance, &code_classe) == 6 && id_lu == id) {
                    strcpy(nom, nom_lu);
                    strcpy(prenom, prenom_lu);
                    retour = 1;
                    break;
                }

        }
        if (!retour) {
            printf("Cet id n'existe pas.\n");
        }
        fclose(fic);

    } while (retour == 0);




    char nom_mat[35];
    do {
        printf("Renseigner la reference de la matiere voulu (ou 0 pour un retour): ");
        scanf("%d", &ref);
        if (ref == 0) return;
        FILE *f = fopen("matieres.csv", "r");
        if(!f) exit(1);
        fgets(chaine, 256, f);


        int retour = 0, verif;
        while (fgets(chaine, sizeof(chaine), f))
        {
            if ((sscanf(chaine, "%d,%[^,],%d", &verif, libelle, &coef) == 3) && (verif == ref)) 
                {   
                    libelle[strcspn(libelle, "\n")] = '\0';  // Nettoyer newline
                    retour = 1;
                    strcpy(nom_mat, libelle);
                    break;
                }

        }
        if (retour == 0) {
            printf("\nMatière introuvable pour cet étudiant. Consulter le menu gestion_des_matieres pour l'ajouter\n");
            return;
        }
        fclose(f);

    } while (retour == 0);


    printf("Confirmez vous vouloir ajouter les notes en %s de l'étudiant %s %s (0)\n", nom_mat, prenom, nom);
    scanf("%d", &verif);
    if (verif != 0) return;


    float cc, ds;
    printf("Note CC : ");
    scanf("%f", &cc);
    printf("\nNote DS : ");
    scanf("%f", &ds);





    FILE *fic = fopen("notes.csv", "a");
    if (!fic) exit(1);

    fprintf(fic, "\n%d,%d,%.2f,%.2f\n", id, ref, cc, ds);
    


    fclose(fic);
    
}

void ajout_notes_etudiant() {

    int retour = 0, id, verif, code = -1;
    char ligne[256], nom[30], prenom[20];

    do {
        printf("Renseigner l'id de l'etudiant voulu (ou 0 pour un retour): ");
        scanf("%d", &id);
        if (id == 0) return;


        FILE *fic_etud = fopen("etudiants.csv", "r");
        if(!fic_etud) exit(1);
        fgets(ligne, 256, fic_etud);

        while (fgets(ligne, sizeof(ligne), fic_etud))
        {
            int id_lu, code_classe;
            char nom_lu[30], prenom_lu[20], email[30], date_naissance[15];
            if ((sscanf(ligne, "%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &id_lu, nom_lu, prenom_lu, email, date_naissance, &code_classe)) == 6 && (id_lu == id)) {
                    strcpy(nom, nom_lu);
                    strcpy(prenom, prenom_lu);
                    code = code_classe;
                    retour = 1;
                    break;
                }
            
        }
        if (retour == 0) {
            printf("❌ Cet id n'existe pas.\n");
        }
        fclose(fic_etud);

    } while (retour == 0);



    if (code == -1) {
        printf("❌ Impossible de trouver le code de la classe pour cet étudiant\n");
        return;
    }
    

    int check = 0;
    float cc, ds;
    int code_classe, ref;
    char nom_classe[10], libelle[35];
    FILE *f = fopen("matieres_classes.csv", "r");
    if (!f) exit(1);

    FILE *fic_notes = fopen("notes.csv", "a");
    if (!fic_notes) exit(1);


    fgets(ligne, sizeof(ligne), f);
    while (fgets(ligne, 256, f))
    {
        if ((sscanf(ligne, "%d,%9[^,],%d,%34[^,\n]", &code_classe, nom_classe, &ref, libelle) == 4) && (code_classe == code)) 
        {
            check = 1;
            printf("\n%s :\n", libelle);
            printf("\nNote CC :");
            scanf("%f", &cc);
            printf("\nNote DS :");
            scanf("%f", &ds);

            fprintf(fic_notes, "\n%d,%d,%.2f,%.2f\n", id, ref, cc, ds);
        }
    }
    
        if (!check) printf("❌ Aucune matière pour la classe de cet étudiant\n");


    fclose(fic_notes);
    fclose(f);


    
}

void ajout_notes_classe() {
    int code_classe[1000], i = 0, ref, verif;
    char nom_classe[10];
    char ligne[256];

    printf("Choisissez le code selon les classes suivantes\n");


    FILE *fic_cls = fopen("classes.csv", "r");
    if (!fic_cls) exit(1);
    
 
        fgets(ligne, sizeof(ligne), fic_cls);
        while (fgets(ligne, 256, fic_cls))
        {
            sscanf(ligne, "%d,%9[^,]", &code_classe[i], nom_classe);
            printf("%10s :\t %3d\n", nom_classe, code_classe[i]);
            i++;
        }
        fclose(fic_cls);


        int code, retourne = 0;
        printf("Code choisi : ");
        scanf("%d", &code);
        for ( i = 0; i < 1000 ; i++)
        {
            if (code_classe[i] == code) retourne = 1;
        }

        if (retourne == 0)
        {
            printf("Ce code est introuvable. Relancez le programme\n");
            return;
        }

       
    

    int retour = 0;
    do
    {
        printf("Donnez la référence de la matière :");
        scanf("%d", &ref);

        FILE *fic_mat = fopen("matieres.csv", "r");
        if (!fic_mat) exit(1);


        fgets(ligne, sizeof(ligne), fic_mat);
        while (fgets(ligne, 256, fic_mat))
        {
            if ((sscanf(ligne, "%d,", &verif) == 1) && (verif == ref)) retour = 1;
        }
        
        fclose(fic_mat);

        if(retour == 0) {
            printf("❌ Référence introuvable. Vérifier et réessayer\n");
            return;
        }
    } while (retour == 0); 



    int id;
    FILE *fic_etud = fopen("etudiants.csv", "r");
    if (!fic_etud) exit(1);

    FILE *f = fopen("notes.csv", "a");
    if (!f) exit(1);
    char line[256];

    fgets(ligne, 256, fic_etud);


    while (fgets(ligne, sizeof(ligne), fic_etud)) {
    int id, code_classe;
    char nom[30], prenom[20], mail[30], date_naissance[15];
    // Lis tous les champs de l'étudiant
    if (sscanf(ligne, "%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", 
               &id, nom, prenom, mail, date_naissance, &code_classe) == 6) {
        if (code_classe == code) {
            float cc, ds;
            printf("\nÉtudiant : %s %s", prenom, nom);
            printf("\nNote CC : ");
            scanf("%f", &cc);
            printf("\nNote DS : ");
            scanf("%f", &ds);
            fprintf(f, "\n%d,%d,%.2f,%.2f", id, ref, cc, ds);
        }
    }
}
    fclose(fic_etud);
    fclose(f);
}





//2
void modification() {
    printf("Bienvenu dans le menu de modification\n");
    int option;
    do
    {
        printf("1- Modifier une note d'un étudiant\n");
        printf("2- Modifier toutes les notes d'un étudiant\n");
        printf("3- Modifier la note des étudiants d'une classe\n");
        printf("0- Retourner au menu précédent\n");
        scanf("%d", &option);
        getchar();

    } while (option < 0 || option > 3);
    

    switch (option)
    {
    case 0:
        return;
        break;
    case 1:
        modification_note_etudiant();
        break;
    case 2:
        modification_notes_etudiant();
        break;
    case 3:
        modification_notes_classe();
        break;
    }
}

void modification_notes_classe() {
    int code_classe[1000], i = 0, ref, verif;
    char nom_classe[10];
    char ligne[256];

    printf("Choisissez le code selon les classes suivantes\n");


    FILE *fic_cls = fopen("classes.csv", "r");
    if (!fic_cls) exit(1);
    
 
        fgets(ligne, sizeof(ligne), fic_cls);
        while (fgets(ligne, 256, fic_cls))
        {
            sscanf(ligne, "%d,%9[^,]", &code_classe[i], nom_classe);
            printf("%10s :\t %3d\n", nom_classe, code_classe[i]);
            i++;
        }
        fclose(fic_cls);


        int code, retourne = 0;
        printf("Code choisi : ");
        scanf("%d", &code);
        for ( i = 0; i < 1000 ; i++)
        {
            if (code_classe[i] == code) retourne = 1;
        }

        if (retourne == 0)
        {
            printf("❌ Ce code est introuvable. Relancez le programme\n");
            return;
        }


    // Vérification de la matière
    retourne = 0;
    do {
        printf("Donnez la référence de la matière :");
        scanf("%d", &ref);

        FILE *fic = fopen("matieres.csv", "r");
        if (!fic) exit(1);
        fgets(ligne, sizeof(ligne), fic); // Sauter l'entête
        while (fgets(ligne, sizeof(ligne), fic)) {
            if (sscanf(ligne, "%d,", &verif) == 1 && (verif == ref)) retourne = 1;
        }
        fclose(fic);

        if (retourne == 0) {
            printf("❌ Référence introuvable. Vérifier et réessayer\n");
        }
    } while (retourne == 0);

    // Lecture des étudiants de la classe et saisie des notes
    typedef struct {
        int id;
        float cc;
        float ds;
    } NoteSaisie;
    NoteSaisie notes_saisies[1000];
    int nb_etudiants = 0;

    FILE *fic_etud = fopen("etudiants.csv", "r");
    if (!fic_etud) exit(1);
    fgets(ligne, sizeof(ligne), fic_etud); // Sauter l'entête

    while (fgets(ligne, sizeof(ligne), fic_etud)) {
        int id, classe_code;
        char nom[30], prenom[20], email[30], date_naissance[15];
        if (sscanf(ligne, "%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &id, nom, prenom, email, date_naissance, &classe_code) == 6) {
            if (classe_code == code) {
                float cc, ds;
                printf("Étudiant : %s %s (id: %d)\n", prenom, nom, id);
                printf("Note CC : ");
                scanf("%f", &cc);
                printf("Note DS : ");
                scanf("%f", &ds);
                notes_saisies[nb_etudiants].id = id;
                notes_saisies[nb_etudiants].cc = cc;
                notes_saisies[nb_etudiants].ds = ds;
                nb_etudiants++;
            }
        }
    }
    fclose(fic_etud);

    // Mise à jour du fichier notes.csv
    FILE *fic_notes = fopen("notes.csv", "r");
    if (!fic_notes) exit(1);
    FILE *fic_temp = fopen("temp.csv", "w");
    if (!fic_temp) exit(1);

    // Copier l'entête
    if (fgets(ligne, sizeof(ligne), fic_notes)) {
        fputs(ligne, fic_temp);
    }

    // Mettre à jour les notes existantes
    while (fgets(ligne, sizeof(ligne), fic_notes)) {
        int note_id, note_ref;
        float note_cc, note_ds;
        if (sscanf(ligne, "%d,%d,%f,%f", &note_id, &note_ref, &note_cc, &note_ds) == 4) {
            int modifie = 0;
            for (int i = 0; i < nb_etudiants; i++) {
                if (note_id == notes_saisies[i].id && note_ref == ref) {
                    fprintf(fic_temp, "\n%d,%d,%.2f,%.2f\n", note_id, ref, notes_saisies[i].cc, notes_saisies[i].ds);
                    notes_saisies[i].id = -1; // Marquer comme traité
                    modifie = 1;
                    break;
                }
            }
            if (!modifie) {
                fputs(ligne, fic_temp);
            }
        } else {
            fputs(ligne, fic_temp);
        }
    }

    // Ajouter les nouvelles notes non encore présentes
    for (int i = 0; i < nb_etudiants; i++) {
        if (notes_saisies[i].id != -1) {
            fprintf(fic_temp, "\n%d,%d,%.2f,%.2f\n", notes_saisies[i].id, ref, notes_saisies[i].cc, notes_saisies[i].ds);
        }
    }

    fclose(fic_notes);
    fclose(fic_temp);

    remove("notes.csv");
    rename("temp.csv", "notes.csv");
    printf("Notes modifiées avec succès pour la classe sélectionnée.\n");
}

void modification_notes_etudiant() {
    int id, verif, retour = 0;
    char nom[30], prenom[20];
    do {
        printf("Renseigner l'id de l'étudiant voulu (ou 0 pour un retour): ");
        scanf("%d", &id);
        if (id == 0) return;
        FILE *fic = fopen("etudiants.csv", "r");
        if(!fic) exit(1);
        char ligne[256];
        fgets(ligne, 256, fic);

        while (fgets(ligne, sizeof(ligne), fic)) {
            if (sscanf(ligne, "%d,%29[^,],%19[^,]", &verif, nom, prenom) == 3 && (verif == id)) {   
                retour = 1;
                break;
            }
        }
        if (retour == 0) {
            printf("Cet id n'existe pas.\n");
        }
        fclose(fic);
    } while (retour == 0);

    printf("Confirmez vous vouloir Modifier les notes de l'etudiant %s %s (0)\n", prenom, nom);
    int verif2;
    scanf("%d", &verif2);
    if (verif2 != 0) return;


    FILE *fic = fopen("notes.csv", "r");
    if (!fic) exit(1);
    FILE *f = fopen("temp.csv", "w");
    if(!f) exit(1);

    char ligne[256];
    // Copier l'entête
    if (fgets(ligne, sizeof(ligne), fic)) {
        fputs(ligne, f);
    }

    int verif_note, ref;
    float cc, ds;
    int modif = 0;

    while (fgets(ligne, sizeof(ligne), fic)) {
        if ((sscanf(ligne, "%d,%d,%f,%f", &verif_note, &ref, &cc, &ds) == 4) && (verif_note == id)) {
            // Pour chaque note de l'étudiant, demander la modification
            char libelle[35] = "";
            int coef, ref_temp;
            FILE *fic_mat = fopen("matieres.csv", "r");
            if (!fic_mat) exit(1);
            char matline[256];
            fgets(matline, sizeof(matline), fic_mat); // Sauter l'entête

            while (fgets(matline, sizeof(matline), fic_mat)) {
                if (sscanf(matline, "%d,%34[^,],%d", &ref_temp, libelle, &coef) == 3 && ref_temp == ref) {
                    libelle[strcspn(libelle, "\n")] = '\0';  // Nettoyer newline
                    printf("%s\n", libelle);
                    printf("Note CC : ");
                    scanf("%f", &cc);
                    printf("Note DS : ");
                    scanf("%f", &ds);
                    modif = 1;

                    break;
                }
            }
            fclose(fic_mat);

            fprintf(f, "\n%d,%d,%.2f,%.2f\n", id, ref, cc, ds);
        } else {
            fputs(ligne, f);
        }
    }

    fclose(fic);
    fclose(f);

    remove("notes.csv");
    rename("temp.csv", "notes.csv");

    if (modif)
        printf("Notes modifiees avec succes pour l'etudiant selectionne.\n");
    else
        printf("Aucune note a modifier pour cet etudiant.\n");
}

void modification_note_etudiant() {

    int id, rep, ref, coef, verif, verif2, verif3, retour = 0;
    char ligne[256], chaine[256];
    char libelle[35], nom [30], prenom[20];
    do {
        printf("Renseigner l'id de l'étudiant voulu (ou 0 pour un retour): ");
        scanf("%d", &id);
        if (id == 0) return;
        FILE *fic = fopen("etudiants.csv", "r");
        if(!fic) exit(1);

        fgets(ligne, 256, fic);
        while (fgets(ligne, sizeof(ligne), fic))
        {
            if ((sscanf(ligne, "%d,%29[^,],%19[^,]", &verif2, nom, prenom) == 3 && (verif2 == id))) 
                {   
                    retour = 1;
                    break;
                }

        }
        if (retour == 0) {
            printf("Cet id n'existe pas.\n");
        }
        fclose(fic);

    } while (retour == 0);


    do {
        printf("Renseigner la reference de la matiere voulu (ou 0 pour un retour): ");
        scanf("%d", &ref);
        if (id == 0) return;


        FILE *f = fopen("matieres.csv", "r");
        if(!f) exit(1);
        fgets(chaine, 256, f);

        retour = 0;
        while (fgets(chaine, sizeof(chaine), f))
        {
            if (sscanf(chaine, "%d,%34[^,],%d", &verif3, libelle, &coef) == 3 && (verif3 == ref)) 
                {   
                    retour = 1;
                    break;
                }

        }
        if (retour == 0) {
            printf("Cette matiere n'existe pas.\nConsulter le menu gestion_des_matieres pour l'ajouter\n");
        }
        fclose(f);

    } while (retour == 0);


    printf("Confirmez vous vouloir Modifier les notes en %s de l'etudiant %s %s (0)\n", libelle, prenom, nom);
    scanf("%d", &verif);
    if (verif != 0) return;


    float cc, ds;
    printf("Note CC : ");
    scanf("%f", &cc);
    printf("\nNote DS : ");
    scanf("%f", &ds);





    FILE *fic = fopen("notes.csv", "r");
    if (!fic) exit(1);

    FILE *f = fopen("temp.csv", "w");
    if(!f) exit(1);


    if (fgets(ligne, sizeof(ligne), fic)) fputs(ligne,f);
    while (fgets(ligne, sizeof(ligne), fic))
    {
        if (sscanf(ligne, "%d,%d,", &verif2, &verif3) == 2 && (verif2 == id && verif3 == ref))
        {
            fprintf(f,"\n%d,%d,%.2f,%.2f", id, ref, cc, ds);
        } else {
            fputs(ligne, f);
        }
        
    }
    
    
    fclose(fic);
    fclose(f);

    remove("notes.csv");
    rename("temp.csv", "notes.csv");
}





//3
void consultation () {
    int rep;
    do {
        printf("Que souhaitez vous faire ?\n");
        printf("1- Consultation d'une note d'un étudiant\n");
        printf("2- Consultation des notes d'un étudiant\n");
        printf("3- Consultation des notes d'une classe dans une matière\n");
        printf("0- Retourner au menu précédent\n");
        scanf("%d", &rep);
        getchar();
        if (rep < 0 || rep > 2) printf("❌ Option invalide\n");
    } while (rep < 0 || rep > 3);



    switch (rep) {
        case 1 : consultation_note_etudiant();
        break;
        case 2: consultation_notes_etudiant();
        break;
        case 3 : consultation_notes_classe();
        break;
        case 0 : return; 
    }
    
}

void consultation_note_etudiant() {
    int id, rep, ref, coef, verif, retour = 0;
    char ligne[256], chaine[256];
    char libelle[35], nom [30], prenom[20];
    do {
        printf("Renseigner l'id de l'étudiant voulu (ou 0 pour un retour): ");
        scanf("%d", &id);


        if (id == 0) return;
        FILE *fic = fopen("etudiants.csv", "r");
        if(!fic) exit(1);
        fgets(ligne, 256, fic);



        while (fgets(ligne, sizeof(ligne), fic)) {
            if (sscanf(ligne, "%d,%29[^,],%19[^,]", &verif, nom, prenom) == 3 && (verif == id)) {   
                retour = 1;
                break;
            }
        }
        if (retour == 0) {
            printf("Cet id n'existe pas.\n");
        }
        fclose(fic);
    } while (retour == 0);

    retour = 0;

    do {
        printf("Renseigner la reference de la matiere voulu (ou 0 pour un retour): ");
        scanf("%d", &ref);
        if (id == 0) return;

        FILE *f = fopen("matieres.csv", "r");
        if(!f) exit(1);

        fgets(chaine, 256, f);

        while (fgets(chaine, sizeof(chaine), f)) {
            if (sscanf(chaine, "%d,%34[^,],%d", &verif, libelle, &coef) == 3 && (verif == ref)) {   
                libelle[strcspn(libelle, "\n")] = '\0';  // Nettoyer newline
                retour = 1;
                break;
            }
        }
        if (retour == 0) {
            printf("\n❌ Cette matière n'existe pas. Consulter le menu gestion_des_matieres pour l'ajouter\n");
            return;
        }
        fclose(f);
    } while (!retour);

    float cc, ds;
    int id_temp, ref_temp;
    int trouve = 0;

    FILE *fic = fopen("notes.csv", "r");
    if (!fic) exit(1);

    char line[256];
    fgets(line, 256, fic); // Sauter l'entête
    while (fgets(line, 256, fic)) {
        if ((sscanf(line, "%d,%d,%f,%f", &id_temp, &ref_temp, &cc, &ds)) == 4 && (id_temp == id) && (ref_temp == ref)) {
            printf("Étudiant : %s %s\n", nom, prenom);
            printf("%s\t Note CC : %.2f\t Note DS : %.2f\n", libelle, cc, ds);
            trouve = 1;
            break;
        }
    }
    if (!trouve) {
        printf("❌ Aucune note trouvée pour cet étudiant dans cette matière.\n");
    }
    fclose(fic);
}

void consultation_notes_etudiant () {
    int id, verif2, retour = 0;
    char nom[30], prenom[20];
    do {
        printf("Renseigner l'id de l'étudiant voulu (ou 0 pour un retour): ");
        scanf("%d", &id);
        if (id == 0) return;
        FILE *fic = fopen("etudiants.csv", "r");
        if(!fic) exit(1);
        char ligne[256];

        fgets(ligne, 256, fic);
        while (fgets(ligne, sizeof(ligne), fic)) {
            if (sscanf(ligne, "%d,%29[^,],%19[^,]", &verif2, nom, prenom) == 3 && (verif2 == id)) {   
                retour = 1;
                break;
            }
        }
        if (retour == 0) {
            printf("Cet id n'existe pas.\n");
        }
        fclose(fic);

    } while (retour == 0);

    int id_etudiant, ref_matiere;
    float note_cc, note_ds;
    char libelle[35];
    FILE *fic = fopen("notes.csv", "r");
    if (!fic) exit(1);

    printf("Notes de %s %s : \n", prenom, nom);
    printf("Matières                        |Note CC          |Note DS        \n");

    char ligne[256];
    int trouve = 0;
    fgets(ligne, sizeof(ligne), fic); // Sauter l'entête
    while (fgets(ligne, 256, fic)) {
        if ((sscanf(ligne, "%d,%d,%f,%f", &id_etudiant, &ref_matiere, &note_cc, &note_ds)) == 4 && (id_etudiant == id)) {
            // Chercher le libellé de la matière
            FILE *fmat = fopen("matieres.csv", "r");
            if (!fmat) exit(1);
            char matline[256];
            int temp;
            fgets(matline, sizeof(matline), fmat); // Sauter l'entête
            while (fgets(matline, sizeof(matline), fmat)) {
                if ((sscanf(matline, "%d,%34[^,]", &temp, libelle)) == 2 && (temp == ref_matiere)) {
                    libelle[strcspn(libelle, "\n")] = '\0';  // Nettoyer newline
                    break;
                }
            }
            fclose(fmat);
            printf("%-35s|%-17.2f|%-17.2f\n", libelle, note_cc, note_ds);
            trouve = 1;
        }
    }
    if (!trouve) {
        printf("❌ Aucune note trouvée pour cet étudiant.\n");
    }
    fclose(fic);
}

void consultation_notes_classe() {
    int code_classe[1000], i = 0, ref, verif;
    char nom_classe[10];
    char ligne[256], chaine[256];

    printf("Choisissez le code parmi les classes suivantes\n");
    FILE *fic_cls = fopen("classes.csv", "r");
    if (!fic_cls) exit(1);
    
 
        fgets(ligne, sizeof(ligne), fic_cls);
        while (fgets(ligne, 256, fic_cls))
        {
            sscanf(ligne, "%d,%9[^,]", &code_classe[i], nom_classe);
            printf("%10s :\t %3d\n", nom_classe, code_classe[i]);
            i++;
        }
        fclose(fic_cls);


        int code, retourne = 0;
        printf("Code choisi : ");
        scanf("%d", &code);
        for ( i = 0; i < 1000 ; i++)
        {
            if (code_classe[i] == code) retourne = 1;
        }

        if (retourne == 0)
        {
            printf("Ce code est introuvable. Relancez le programme\n");
            return;
        }

    // Vérification de la matière
    retourne = 0;
    do {
        printf("Donnez la reference de la matiere :");
        scanf("%d", &ref);

        FILE *fic = fopen("matieres.csv", "r");
        if (!fic) exit(1);


        fgets(ligne, sizeof(ligne), fic); // Sauter l'entête
        while (fgets(ligne, sizeof(ligne), fic)) {
            if (sscanf(ligne, "%d,", &verif) == 1 && (verif == ref)) retourne = 1;
        }
        fclose(fic);

        if (retourne == 0) {
            printf("Reference introuvable. Verifier et reesayer\n");
        }
    } while (retourne == 0);







    printf("\n%-30s %-30s : %-10s | %-10s\n", "Prenom", "Nom", "CC", "DS");
    printf("-------------------------------------------------------------------------------\n");


    // Charger tous les id des étudiants de la classe
    int ids[1000], nb_ids = 0;
    FILE *fic_etud = fopen("etudiants.csv", "r");
    if (!fic_etud) exit(1);

    fgets(ligne, sizeof(ligne), fic_etud); // Sauter l'entête
    while (fgets(ligne, sizeof(ligne), fic_etud)) {
        int id, classe_code;
        char nom[30], prenom[20], email[30], date_naissance[15];
        if (sscanf(ligne, "%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &id, nom, prenom, email, date_naissance, &classe_code) == 6) {
            if (classe_code == code) {
                ids[nb_ids++] = id;
                FILE *fic_notes = fopen("notes.csv", "r");
                if (!fic_notes) exit(1);
    

                // Copier l'entête
                fgets(chaine, sizeof(chaine), fic_notes); 

                while (fgets(chaine, sizeof(chaine), fic_notes)) {
                    int id_note, ref_note;
                    float note_cc, note_ds;
                    if ((sscanf(chaine, "%d,%d,%f,%f", &id_note, &ref_note, &note_cc, &note_ds) == 4) && (ref_note == ref)) {
                    
                            for (int j = 0; j < nb_ids; j++) {
                                    if (id_note == ids[j]) {
                                        printf("%-30s %-30s : %-10.2f | %-10.2f\n", prenom, nom, note_cc, note_ds);
                                        break;
                                    }
                            }

                    }
                    
                }
                fclose(fic_notes);
                
            }
        }
    }
    fclose(fic_etud);

    

}





//4
void suppression() {
     int rep;
    do {
        printf("Que souhaitez vous faire ?\n");
        printf("1- Suppression d'une note d'un étudiant\n");
        printf("2- Suppression des notes d'un étudiant\n");
        printf("3- Suppression des notes d'une classe dans une matière\n");
        printf("0- Retourner au menu précédent\n");
        scanf("%d", &rep);
        getchar();
        if (rep < 0 || rep > 3) printf("❌ Option invalide\n");
    } while (rep < 0 || rep > 3);



    switch (rep) {
        case 1 : suppression_note_etudiant();
        break;
        case 2: suppression_notes_etudiant();
        break;
        case 3 : suppression_notes_classe();
        break;
        case 0 : return; 
    }

}

void suppression_note_etudiant() {


    int id, rep, ref, coef, verif, verif2, verif3, retour = 0;
    char ligne[256], chaine[256];
    char libelle[35], nom [30], prenom[20];
    do {
        printf("Renseigner l'id de l'étudiant voulu (ou 0 pour un retour): ");
        scanf("%d", &id);
        if (id == 0) return;
        FILE *fic = fopen("etudiants.csv", "r");
        if(!fic) exit(1);

        fgets(ligne, 256, fic);
        while (fgets(ligne, sizeof(ligne), fic))
        {
            if ((sscanf(ligne, "%d,%29[^,],%19[^,]", &verif2, nom, prenom) == 3 && (verif2 == id))) 
                {   
                    retour = 1;
                    break;
                }

        }
        if (retour == 0) {
            printf("Cet id n'existe pas.\n");
        }
        fclose(fic);

    } while (retour == 0);


    do {
        printf("Renseigner la reference de la matiere voulu (ou 0 pour un retour): ");
        scanf("%d", &ref);
        if (id == 0) return;


        FILE *f = fopen("matieres.csv", "r");
        if(!f) exit(1);
        fgets(chaine, 256, f);

        retour = 0;
        while (fgets(chaine, sizeof(chaine), f))
        {
            if (sscanf(chaine, "%d,%34[^,],%d", &verif3, libelle, &coef) == 3 && (verif3 == ref)) 
                {   
                    libelle[strcspn(libelle, "\n")] = '\0';  // Nettoyer newline
                    retour = 1;
                    break;
                }

        }
        if (retour == 0) {
            printf("Cette matiere n'existe pas.\nConsulter le menu gestion_des_matieres pour l'ajouter\n");
        }
        fclose(f);

    } while (retour == 0);


    printf("Confirmez vous vouloir supprimer les notes en %s de l'étudiant %s %s (0)\n", libelle, prenom, nom);
    scanf("%d", &verif);
    if (verif != 0) return;



    FILE *fic = fopen("notes.csv", "r");
    if (!fic) exit(1);

    FILE *f = fopen("temp.csv", "w");
    if(!f) exit(1);


    if (fgets(ligne, sizeof(ligne), fic)) fputs(ligne,f);
    while (fgets(ligne, sizeof(ligne), fic))
    {
        if (sscanf(ligne, "%d,%d,", &verif2, &verif3) == 2 && (verif2 != id || verif3 != ref))
        {
            fputs(ligne, f);
        }
        
    }
    
    
    fclose(fic);
    fclose(f);

    remove("notes.csv");
    rename("temp.csv", "notes.csv"); 
    printf("✓ Suppression réussie\n");
}

void suppression_notes_etudiant() {

 int id, rep, ref, coef, verif, verif2, verif3, retour = 0;
    char ligne[256], chaine[256];
    char nom [30], prenom[20];
    do {
        printf("Renseigner l'id de l'étudiant voulu (ou 0 pour un retour): ");
        scanf("%d", &id);
        if (id == 0) return;
        FILE *fic = fopen("etudiants.csv", "r");
        if(!fic) exit(1);

        fgets(ligne, 256, fic);
        while (fgets(ligne, sizeof(ligne), fic))
        {
            if ((sscanf(ligne, "%d,%29[^,],%19[^,]", &verif2, nom, prenom) == 3 && (verif2 == id))) 
                {   
                    retour = 1;
                    break;
                }

        }
        if (retour == 0) {
            printf("Cet id n'existe pas.\n");
        }
        fclose(fic);

    } while (retour == 0);




    printf("Confirmez vous vouloir Modifier les notes de l'étudiant %s %s (0)\n", prenom, nom);
    scanf("%d", &verif);
    if (verif != 0) return;





    FILE *fic = fopen("notes.csv", "r");
    if (!fic) exit(1);

    FILE *f = fopen("temp.csv", "w");
    if(!f) exit(1);


    if (fgets(ligne, sizeof(ligne), fic)) fputs(ligne,f);
    while (fgets(ligne, sizeof(ligne), fic))
    {
        if (sscanf(ligne, "%d,%d,", &verif2, &verif3) == 2 && (verif2 != id))
        {
            fputs(ligne, f);
        }
        
    }
    
    
    fclose(fic);
    fclose(f);

    remove("notes.csv");
    rename("temp.csv", "notes.csv");
    printf("✓ Suppression réussie\n");
}

void suppression_notes_classe() {

    int code_classe[1000], i = 0, ref, verif;
    char nom_classe[10];
    char ligne[256];

    printf("Choisissez le code parmi les classes suivantes\n");
    FILE *fic_cls = fopen("classes.csv", "r");
    if (!fic_cls) exit(1);
    
 
        fgets(ligne, sizeof(ligne), fic_cls);
        while (fgets(ligne, 256, fic_cls))
        {
            sscanf(ligne, "%d,%9[^,]", &code_classe[i], nom_classe);
            printf("%10s :\t %3d\n", nom_classe, code_classe[i]);
            i++;
        }
        fclose(fic_cls);


        int code, retourne = 0;
        printf("Code choisi : ");
        scanf("%d", &code);
        for ( i = 0; i < 1000 ; i++)
        {
            if (code_classe[i] == code) retourne = 1;
        }

        if (retourne == 0)
        {
            printf("Ce code est introuvable. Relancez le programme\n");
            return;
        }

    // Vérification de la matière
    retourne = 0;
    do {
        printf("Donnez la référence de la matière :");
        scanf("%d", &ref);

        FILE *fic = fopen("matieres.csv", "r");
        if (!fic) exit(1);
        char ligne[256];
        fgets(ligne, sizeof(ligne), fic); // Sauter l'entête
        while (fgets(ligne, sizeof(ligne), fic)) {
            if (sscanf(ligne, "%d,", &verif) == 1 && (verif == ref)) retourne = 1;
        }
        fclose(fic);

        if (retourne == 0) {
            printf("❌ Référence introuvable. Vérifier et réessayer\n");
        }
    } while (retourne == 0);





    // Charger tous les id des étudiants de la classe
    int ids[1000], nb_ids = 0;
    FILE *fic_etud = fopen("etudiants.csv", "r");
    if (!fic_etud) exit(1);

    fgets(ligne, sizeof(ligne), fic_etud); // Sauter l'entête

    while (fgets(ligne, sizeof(ligne), fic_etud)) {
        int id, classe_code;
        char nom[30], prenom[20], email[30], date_naissance[15];
        if (sscanf(ligne, "%d,%29[^,],%19[^,],%29[^,],%14[^,],%d", &id, nom, prenom, email, date_naissance, &classe_code) == 6) {
            if (classe_code == code) {
                ids[nb_ids++] = id;
            }
        }
    }
    fclose(fic_etud);

    // Suppression dans notes.csv
    FILE *fic_notes = fopen("notes.csv", "r");
    if (!fic_notes) exit(1);
    FILE *fic_tmp = fopen("temp.csv", "w");
    if (!fic_tmp) exit(1);

    // Copier l'entête
    if (fgets(ligne, sizeof(ligne), fic_notes)) {
        fputs(ligne, fic_tmp);
    }

    while (fgets(ligne, sizeof(ligne), fic_notes)) {
        int id_note, ref_note;
        float note_cc, note_ds;
        if (sscanf(ligne, "%d,%d,%f,%f", &id_note, &ref_note, &note_cc, &note_ds) == 4) {
            int est_etudiant_classe = 0;
            for (int i = 0; i < nb_ids; i++) {
                if (id_note == ids[i]) {
                    est_etudiant_classe = 1;
                    break;
                }
            }
            // On ne copie pas la note si c'est un étudiant de la classe ET la matière correspond
            if (est_etudiant_classe && ref_note == ref) {
                continue;
            }
        }
        fputs(ligne, fic_tmp);
    }
    fclose(fic_notes);
    fclose(fic_tmp);

    remove("notes.csv");
    rename("temp.csv", "notes.csv");
    printf("✓ Toutes les notes de la classe %d dans la matière %d ont été supprimées.\n", code, ref);
}