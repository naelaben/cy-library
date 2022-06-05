# cy-library

Pour compiler le programme: 
make

et executer:
./library

Autres commandes make:

make clean
pour supprimer des fichiers objets.

make fclean 
pour supprimer les fichiers objets et binaires.

À propos des fichiers.
Il existe 3 fichiers principaux :
/!\ Ces trois fichiers DOIVENT exister lors de l'exécution du programme.
/!\ Il n'est pas possible d'avoir des lignes vides dans les fichiers.


- livres : contient la base de données des livres.
Les livres sont stockés au format : "titre"[espace]"auteur"[espace]"id"[espace]"genre"


- utilisateurs : contient la liste des utilisateurs et des informations les concernant.
Ils sont stockés au format : nom d'utilisateur[espace]mot de passe[espace]rôle

- livres_empruntés : Il contient la liste des livres empruntés par les utilisateurs.
Ils sont stockés au format : nom d'utilisateur[espace]id[heure]

Il existe un fichier temporaire qui est également créé lors du rendu d'un livre.
Ce fichier est censé être supprimé, il n'apparaît donc jamais.
Cependant, il apparaît sous le nom temp s'il y a un problème lors de l'exécution.
