#include "piece.h"

//Creer une nouvelle pi�ce aveec un joueur et un statut
struct Piece piece_creer(int joueur, bool statut)
{
	struct Piece piece;
	piece.joueur = joueur;
	piece.status = statut;
	return piece;
}

//Identifier la pi�ce d'apr�s un caract�re
struct Piece piece_identifier(char piece)
{
	struct Piece resultat;

	switch (piece)
	{
	case 'p':
		//Piece du joueur 1 non promu
		resultat.joueur = 0;
		resultat.status = false;
		break;

	case 'd':
		//Piece du joueur 1 promu
		resultat.joueur = 0;
		resultat.status = true;
		break;

	case 'P':
		//Piece du joueur 2 non promu
		resultat.joueur = 1;
		resultat.status = false;
		break;

	case 'D':
		//Piece du joueur 2 promu
		resultat.joueur = 1;
		resultat.status = true;
		break;

	default:
		//En cas d'un caractere non valide le joueur est �gale � '-1'
		//Les caract�res non valide sans l'espace ' ' et le point '.' qui peuvent �tre trouver dans le damier
		//Le joueur '-1' servira comme une methode pour verifier les piece dans le damier
		resultat.joueur = -1;
		resultat.status = false;
		break;
	}

	return resultat;
}

//Afficher la pi�ce dans l'ecran
void piece_afficher(struct Piece piece)
{
	printf("%c", piece_caractere(piece));
}

//Renvoyer le caract�re correspondant � la pi�ce
char piece_caractere(struct Piece piece)
{
	char resultat;

	if (piece.status)
	{
		//Piece promu
		resultat = 'd';
	}
	else {
		//Piece non promu
		resultat = 'p';
	}

	if (piece_joueur(piece) == 1)
	{
		//Si le joueur est le deuxieme joueur en change le charactere en majuscule
		resultat = (char)toupper((int)resultat);
	}

	return resultat;
}

//Renvoyer le joueur correspondant � la pi�ce
int piece_joueur(struct Piece piece)
{
	return piece.joueur;
}