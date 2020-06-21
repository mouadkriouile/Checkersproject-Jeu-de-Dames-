#include "general.h"

//Une fontion pour pauser, entree pour continuer
void attendre(void)
{
	char buffer[8]; // temporary
	fgets(buffer, sizeof(buffer), stdin);
}

//Une fontion pour effacer l'ecran
void effacer_ecran(void)
{
	system(CLEAR);
}

//Degager la valeur du caractere digit
int caractere_valeur(char c)
{
	switch (c)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return 0;
		break;
	}
}

//Minimiser et compter les caractere de la chaine
int nettoyer_chaine(char *chaine, int taille)
{
	int caractere_courant = 0;
	int caractere_compteur = 0;

	for (caractere_courant = 0; caractere_courant < taille; caractere_courant++)
	{
		//Cararctere traité
		caractere_compteur++;

		//Si on trouve '\n' on la remplace par '\0' 
		if (chaine[caractere_courant] == '\n')
		{
			chaine[caractere_courant] = '\0';
			break;
		}

		chaine[caractere_courant] = tolower(chaine[caractere_courant]);
	}

	return caractere_compteur;
}

//Creer une nouvelle coordonnée à partir de deux position X et Y
struct coord coord_creer(int x, int y)
{
	struct coord c;
	c.x = x;
	c.y = y;
	return c;
}

//Degager la direction du mouvement
struct coord direction_du_mouvement(struct coord initial, struct coord finale)
{
	struct coord facteur;

	facteur.x = finale.x - initial.x;
	facteur.y = finale.y - initial.y;

	//Si la valeur absolue des deux difference ne sont pas ègeaux
	//Alors le deplacement est strictement non diagonale
	if (abs(facteur.x) != abs(facteur.y))
	{
		//En renvoie le facteur comme il est (inégal)
		return facteur;
	}

	if (facteur.x > 0)
	{
		facteur.x = 1; //Direction vers la droite
	}
	else if (facteur.x < 0)
	{
		facteur.x = -1; //Direction vers la gauche
	}

	if (facteur.y > 0)
	{
		facteur.y = 1; //Direction vers le haut
	}
	else if (facteur.y < 0)
	{
		facteur.y = -1; //Direction vers le bas
	}

	//Si le facteur X ou Y est 0 il le reste, car le pion n'a pas changer dans les deux axes
	return facteur;
}
