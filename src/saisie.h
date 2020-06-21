#pragma once
#include "general.h"
#include "partie.h"

//Resultat renvoyer apres avoir reçu une commande
typedef struct Resultat_saisie
{
	bool  commande_valide;
	char *commande_chaine;

	int nombre_coordonnees;
	struct coord* liste_coordonnees;

} Resultat_saisie;

//Attendre la commande de l'utilisateur
struct Resultat_saisie saisie_case(struct Partie *partie);
