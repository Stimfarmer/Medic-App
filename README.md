# PROJET_SYSTEME_RESEAUX
Voilà le Git de dépôt pour notre projet de système et réseaux.
Le projet est écrit en langage C.
Aucune licence n'est définie.


# Lignes de compilation:
gcc -Wall -o ssl-client clientssl.c chaine.c strdup.c strsplit.c cryptage.c -L/usr/local/ssl/lib/ -lcrypto -lssl; gcc -Wall -o ssl-serveur serveurssl.c fonction_serveur.c strdup.c strsplit.c fonction_client_on_serveur.c chaine.c account.c cryptage.c -L/usr/local/ssl/lib/ -lcrypto -lssl -lpthread

# vim : 
vim déclenche en fait nano --> pas de lag au lancement !
plus besoin de changer le directory à chaque fois, c'est devenu automatique !!
vim_script.sh : mettre le bon nom de user pour la commande ssh
fonction_serveur.c : ligne 24, la chaine vim_pass doit contenir le mot de passe de la session qui execute ssh


