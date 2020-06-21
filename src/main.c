#include "general.h"
#include "partie.h"

//La fonction principale
int main(int argc, char *argv[])
{
	//Declarer un pointer pour la partie
	struct Partie *partie = NULL;
	char En_Tete[MAX_BUFFER];
	FILE *fichier;

	//effacer ecran
	effacer_ecran();

	if (argc >= 2)
	{
		//Detecter le type du fichier

		printf("Chargement : %s\n", argv[1]);

		//Ouvrir le fichier
		fichier = fopen(argv[1], "r");

		if (fichier)
		{
			//Degager l'en-tête du fichier
			fscanf(fichier, "%s", En_Tete);

			//Verifier l'en_tête du fichier
			if (strcmp(En_Tete, "PL") == 0)
			{
				//Si il s'agit d'un fichier plateau 'PLT'
				partie = partie_charger(fichier);
				if (partie != NULL)
				{
					partie_jouer(partie);
				}
			}
			else if (strcmp(En_Tete, "PR") == 0)
			{
				//Si il s'agit d'un fichier partie 'PART'
				partie = replay_charger(fichier);
				if (partie != NULL)
				{
					replay_jouer(partie);
				}
			}
			else {
				//Afficher une erreur
				printf("\nLe fichier '%s' est invalide car l'en-tete du fichier n'est pas valide.\n", argv[1]);

				//Fermer le fichier
				fclose(fichier);
			}
		}
		else {
			//Afficher une erreur
			printf("\nLe programe n'a pas pu ouvrir le fichier '%s'!\n", argv[1]);
		}
	}
	else {
		//Demarrer une nouvelle partie
		partie = partie_nouvelle();

		if (partie != NULL)
		{
			partie_jouer(partie);
		}
	}

	//JEU TERMINE : CLEAN UP

	//Detruire la partie si elle n'est pas NULL
	if (partie != NULL)
	{
		partie_detruire(partie);
	}

	//Terminer
	printf("\n\nProgramme termine, veuillez cliquer ENTREE pour quitter !");
	attendre();

	return 0;
}