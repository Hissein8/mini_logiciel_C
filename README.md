# first_public_repository_in_github
# Mini Logiciel de Gestion Étudiante (Langage C)

## Présentation
Ce projet est une application console développée en **C** permettant de gérer administrativement des étudiants, des classes et des notes. Il a été conçu pour mettre en pratique la gestion rigoureuse de la mémoire et la manipulation de fichiers plats (persistance des données).

## Fonctionnalités principales
* **Gestion des Étudiants :** Ajout, modification et suppression d'enregistrements.
* **Gestion des Classes :** Organisation des étudiants par cohortes.
* **Système de Notes :** Saisie des notes et calcul automatique des moyennes.
* **Persistance :** Sauvegarde et lecture des données via des fichiers `.csv` pour conserver les informations après la fermeture du programme.

## Concepts Techniques Validés
* **Structures (struct) :** Organisation de données complexes.
* **Tableaux & Pointeurs :** Manipulation efficace des listes en mémoire.
* **Fichiers (stdio.h) :** Implémentation d'une base de données simple via le système de fichiers.
* **Algorithmie :** Recherche, tri et filtrage de données.

## Installation et Test
1. **Prérequis :** Un compilateur C (GCC recommandé).
2. **Clonage :**
    ```bash
    git clone https://github.com/Hissein8/mini_logiciel_C.git
    cd mini_logiciel_C
3. **compilation**
    gcc *.c -o gestion_etudiant
4. **exécution**
    ./gestion_etudiant