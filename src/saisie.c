#include "saisie.h"

//Attendre la commande de l'utilisateur
struct Resultat_saisie saisie_case(struct Partie *partie)
{
	//Declarer les variables n�cessaires 
	struct Resultat_saisie resultat;
	struct coord position_courante;

	bool commande_obtenue;
	char commande[MAX_BUFFER];

	int caractere_courant,
		nombre_caractere,
		nombre_mouvement,
		lecture_erreur,
		lecture_etape;

	//Initialiser les variables
	resultat.liste_coordonnees = (struct coord*)malloc(MAX_DEPLACEMENT * sizeof(sizeof(struct coord)));
	resultat.commande_valide = false;
	resultat.commande_chaine = NULL;
	commande_obtenue = false;
	nombre_caractere = 0;

	//Capturer les commandes des joueurs
	while (!commande_obtenue)
	{
		printf("JOUEUR %i > ", ((int)partie->joueur + 1));

		//Lire la commande de l'utilisateur
		fgets(commande, sizeof(commande), stdin);

		//Nettoyer la chaine
		nombre_caractere = nettoyer_chaine(commande, sizeof(commande));

		//Les commandes doivent commencer par un slash '/' - Exemple: /fermer
		//On verifie si c'est une commande
		if (isalpha(commande[0]))
		{
			//Allouer la m�moire et coupier la commande de dans
			resultat.commande_chaine = (char*)malloc((nombre_caractere) * sizeof(char));
			memcpy(resultat.commande_chaine, commande, nombre_caractere);

			return resultat;
		}

		//Initialiser les variables
		nombre_mouvement = 0;
		lecture_erreur = 0;
		lecture_etape = 0;

		for (caractere_courant = 0; caractere_courant < sizeof(commande); caractere_courant++)
		{
			switch (lecture_etape)
			{
			case 0: //Lecture du coordonn�e X
				if (isdigit(commande[caractere_courant]))
				{
					position_courante.x = caractere_valeur(commande[caractere_courant]);
					lecture_etape = 1;
				}
				else
				{
					//Si c'est la fin de la chaine des coordonn�es alors ce n'est pas une erreur de lecture
					if (commande[caractere_courant] != '\0')
					{
						lecture_erreur = true;
					}
				}
				break;

			case 1: //Lecture du coordonn�e Y
				if (isdigit(commande[caractere_courant]))
				{
					position_courante.y = caractere_valeur(commande[caractere_courant]);
					lecture_etape = 2;
				}
				else
				{
					lecture_erreur = true;
				}
				break;

			case 2: //Lecture de l'espace ou fin de la chaine '\0' alors on enregistre les coordonn�es re�us
				if (commande[caractere_courant] == ' ' || commande[caractere_courant] == '\0')
				{
					//Enregistrer les coordon�es et ajouter 1 au nombre de mouvement
					resultat.liste_coordonnees[nombre_mouvement] = position_courante;
					nombre_mouvement++;

					//R�initialiser l'etape de lecture
					lecture_etape = 0;
				}
				else
				{
					lecture_erreur = true;
				}
				break;
			}

			//Si c'est la fin du buffer 'commande' et on a au moins 2 coordon�es 
			//une de la position initiale et l'autre du finale on sort du boucle de lecture
			if (commande[caractere_courant] == '\0')
			{
				if (nombre_mouvement >= 2)
				{
					//Commande re�ue alors en sort du boucle g�n�rale
					commande_obtenue = true;
				}
				else
				{
					//On sort du boucle de lecture
					resultat.commande_valide = false;
					return resultat;
				}

				break;
			}

			if (lecture_erreur == true)
			{
				//On sort du boucle de lecture
				resultat.commande_valide = false;
				return resultat;
			}
		}
	}

	//enregistrer le nombre de coordonn�es trouv�es dans la commande
	resultat.nombre_coordonnees = nombre_mouvement;

	//Renvoyer le resultat
	resultat.commande_valide = true;
	return resultat;
}