cmake_minimum_required(VERSION 3.0)

project(PROJET_SYSTEME_RESEAUX)

set(SRCS_serveur
	SOURCES/serveurssl.c
	SOURCES/fonction_serveur.c
	SOURCES/strdup.c
	SOURCES/strsplit.c
	SOURCES/fonction_client_on_serveur.c
	SOURCES/chaine.c
	SOURCES/account.c
	SOURCES/cryptage.c
	)

add_executable(serveurssl ${SRCS_serveur})

target_link_libraries(serveurssl
        crypto
        ssl
        pthread 
)

set(SRCS_client
	SOURCES/clientssl.c
	SOURCES/chaine.c
	SOURCES/strdup.c
	SOURCES/strsplit.c
	SOURCES/cryptage.c
	)

add_executable(clientssl ${SRCS_client})

target_link_libraries(clientssl
	crypto
	ssl
)


	
