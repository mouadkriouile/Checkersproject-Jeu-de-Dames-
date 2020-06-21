#pragma once

#include "general.h"

//Structure qui represente une piece
typedef struct Piece
{
	//Joueur 0: le premier joueur
	//Joueur 1: Le deuxième joueur
	int joueur;

	//False: Piece non promu
	//True: Piece promu
	bool status;

} Piece;

//Structure qui represente une piece capturée
typedef struct Piece_capture
{
	//Coordonnées de la pièce capturée
	struct coord pos;

	//Le status promu ou non promu
	bool status;

} Piece_capture;

/*
	Declaration des fonctions de gestion de partie
*/

struct Piece piece_creer(int joueur, bool statut);
struct Piece piece_identifier(char piece);

void piece_afficher(struct Piece piece);

char piece_caractere(struct Piece piece);
int piece_joueur(struct Piece piece);
