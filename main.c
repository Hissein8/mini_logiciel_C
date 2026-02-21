#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_des_etudiants.h"
#include "gestion_des_classes.h"
#include "gestion_des_matieres.h"
#include "gestion_des_notes.h"



int main(void) {
    short rep;
    printf("\n===== ACCUEIL =====\n");
    printf("Bienvenue dans notre application dédiée à la gestion des ressources académiques.\n");
    do {
        printf("\n----- MENU PRINCIPAL -----\n");
        printf("1. Gestion des etudiants\n");
        printf("2. Gestion des classes\n");
        printf("3. Gestion des matieres\n");
        printf("4. Gestion des notes\n");
        printf("0. Arreter le programme\n");
        printf("---------------------------\n");
        printf("\nVotre choix : ");
        scanf("%hd", &rep);
        getchar();

        switch (rep)
        {
        case 1: gestion_des_etudiants();
            break;
        case 2: gestion_des_classes();
            break;
        case 3: gestion_des_matieres();
            break;
        case 4: gestion_des_notes();
            break;
        case 0: printf("\n✓ Au revoir !\n\n");
            break;
        default: printf("\n❌ Option invalide\n");
            break;
        } 
    }   while (rep != 0);
    return 0;
}