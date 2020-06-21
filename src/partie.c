#include "partie.h"

//Allouer la m�moire pour heberger une partie
struct Partie *partie_creer()
{
	struct Partie *partie = (struct Partie*) malloc(sizeof(struct Partie));

	//Initialiser la liste des coups jou�s
	partie->coups_joues.nombre_deplacements = 0;
	partie->coups_joues.eDebut = NULL;
	partie->coups_joues.eFinal = NULL;

	//Initialiser la liste des configuration
	partie->configuration.nombre_configuration = 0;
	partie->configuration.eDebut = NULL;
	partie->configuration.eFinal = NULL;

	//Initialiser les variables
	partie->plateau_charge = false;
	partie->joueur = false;

	return partie;
}

//Cr�er une nouvelle partie
struct Partie *partie_nouvelle()
{
	//Allou� une partie de m�moire pour la partie
	struct Partie *partie;
	int ligne_courante;
	int colonne_courante;

	//Creer une partie
	partie = partie_creer();

	//Initialiser le damier
	for (ligne_courante = 0; ligne_courante < 10; ligne_courante++)
	{
		for (colonne_courante = 0; colonne_courante < 10; colonne_courante++)
		{
			if (colonne_courante % 2 == 0)
			{
				if (ligne_courante % 2 == 0)
				{
					if (ligne_courante < 4)
					{
						partie->damier[colonne_courante][ligne_courante] = piece_caractere(piece_creer(0, false));
					}
					else if (ligne_courante > 5) {
						partie->damier[colonne_courante][ligne_courante] = piece_caractere(piece_creer(1, false));;
					}
					else {
						partie->damier[colonne_courante][ligne_courante] = '.';
					}
				}
				else {
					partie->damier[colonne_courante][ligne_courante] = ' ';
				}
			}
			else {
				if (ligne_courante % 2 != 0)
				{
					if (ligne_courante < 4)
					{
						partie->damier[colonne_courante][ligne_courante] = piece_caractere(piece_creer(0, false));;
					}
					else if (ligne_courante > 5) {
						partie->damier[colonne_courante][ligne_courante] = piece_caractere(piece_creer(1, false));;
					}
					else {
						partie->damier[colonne_courante][ligne_courante] = '.';
					}
				}
				else {
					partie->damier[colonne_courante][ligne_courante] = ' ';
				}
			}
		}
	}

	return partie;
}

//Charger un plateau � partir d'un fichier plateau
struct Partie *partie_charger(FILE *fichier)
{
	//Allou� une partie de m�moire pour la partie
	struct Partie *partie;
	char caractere_courant;

	int colonne_courante = 0,
		ligne_courante = 9;

	//Creer une partie
	partie = partie_creer();

	//Le plateau a �t� charg� d'apres un fichier 
	partie->plateau_charge = true;

	while (true)
	{
		fscanf(fichier, "%1c", &caractere_courant);

		if (caractere_courant == 'P' || caractere_courant == 'p' ||
			caractere_courant == 'D' || caractere_courant == 'd' ||
			caractere_courant == '.' || caractere_courant == ' ')
		{
			partie->damier[colonne_courante][ligne_courante] = caractere_courant;
			colonne_courante++;

			if (colonne_courante > 9)
			{
				colonne_courante = 0;
				ligne_courante--;

				if (ligne_courante < 0)
				{
					break;
				}
			}

		}
		else if (caractere_courant != '\n')
		{
			//Afficher une erreur car un caract�re invalide a �t� trouv�
			printf("Le fichier que vous avez entre est errone !\n");

			//Detruire la partie creer
			partie_detruire(partie);

			//La partie est null
			partie = NULL;

			//Sortir
			break;
		}
	}

	//Fermer le fichier
	fclose(fichier);

	//Renvoyer la partie
	return partie;
}

//Charger le fichier partie qui contient une replay
struct Partie *replay_charger(FILE *fichier)
{
	struct Partie *partie;
	struct coord *liste_deplacement;
	struct Deplacement *deplacement;

	int nombre_deplacement,
		caracteres_lus,
		coord_courant;

	//Initialiser les variable
	liste_deplacement = NULL;
	nombre_deplacement = 0;
	coord_courant = 0;

	//Creer une nouvelle partie
	partie = partie_nouvelle();

	while (true)
	{
		//Degager le nombre des deplacement intermediaire
		caracteres_lus = fscanf(fichier, "%d", &nombre_deplacement);

		//Si c'est la fin du fichier
		if (caracteres_lus == EOF)
			break;

		printf("%i\n", nombre_deplacement);

		//Allouer la memoire pour enregistrer les deplacement
		liste_deplacement = (struct coord*) malloc(nombre_deplacement * sizeof(struct coord));

		//Lire tous les coordonn�es et les enregistr�es dans la liste des deplacements
		for (coord_courant = 0; coord_courant < nombre_deplacement; coord_courant++)
		{
			fscanf(fichier, "%1d%1d", &liste_deplacement[coord_courant].x,
				&liste_deplacement[coord_courant].y);
		}

		//Allouer la memoire pour un deplacement
		deplacement = (struct Deplacement*) malloc(sizeof(struct Deplacement));

		//Verifier le mouvement
		if (deplacement_valide(partie, liste_deplacement, nombre_deplacement, deplacement))
		{
			//Modifier le damier et ajouter le deplacement dans la liste des coups jou�s
			modif_damier(partie, deplacement);

			//Changer le joueur
			changer_joueur(partie);

			// ATTENTION :
			// On ne lib�re pas 'deplacement'  car il est maintenant
			// �lement de la liste doublement chain� des coups jou�s
		}
		else
		{
			//Afficher une erreur
			printf("Le fichier que vous avez entre est errone !\n");

			//Liberer la m�moire reserv� au 'deplacement' car il n'est pas valide et non ajout�
			free(deplacement);

			//Detruire ce qui est deja charg� dans partie
			partie_detruire(partie);

			//Fichier erronn�
			partie = NULL;

			//Sortir de la boucle
			break;
		}
	}

	//Liberer la m�moire reserv� � la liste des deplacements
	free(liste_deplacement);

	//Fermer le fichier
	fclose(fichier);

	//Renvoyer le pointeur vers la partie
	return partie;
}

//Afficher le plateau d'apr�s le damier courant ou une configuration pr�cedente
void afficher_plateau(struct Partie* partie, struct Configuration *config_a_afficher)
{
	int ligne_courante = 0;
	int	colonne_courante = 0;

	printf("\t\t   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |\n");
	printf("\t\t---+---+---+---+---+---+---+---+---+---+---+---\n");

	for (ligne_courante = 9; ligne_courante >= 0; ligne_courante--)
	{
		printf("\t\t %i |", ligne_courante);

		for (colonne_courante = 0; colonne_courante < 10; colonne_courante++)
		{
			if (config_a_afficher == NULL)
			{
				printf(" %c |", partie->damier[colonne_courante][ligne_courante]);
			}
			else {
				printf(" %c |", config_a_afficher->damier[colonne_courante][ligne_courante]);
			}
		}

		printf(" %i \n\t\t---+---+---+---+---+---+---+---+---+---+---+---\n", ligne_courante);

		//printf(" %i\n", ligne_courante);
	}

	printf("\t\t   | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |\n\n");

}

//modifier une case du damier
void modifier_case(struct Partie *partie, struct Piece piece, struct coord position)
{
	//Modifier la case par la piece demand�e
	partie->damier[position.x][position.y] = piece_caractere(piece);
}

//Modifier le damier et ajouter le coups jou� et son configuration aux listes doublements chainn�es
void modif_damier(struct Partie *partie, struct Deplacement *deplacement)
{
	//Declaration des variables n�cessaire
	struct Configuration *configuration;
	struct Piece piece_courante;
	int compteur;

	//Allouer la memoire pour un element de configuration 
	configuration = malloc(sizeof(struct Configuration));

	//Le deplacement est suppos� valide alors :
	//On enregistre le damier dans la liste de configuration avant de la modifier
	memcpy(configuration->damier, partie->damier, sizeof(char) * 10 * 10);

	//Degager la piece jou�
	piece_courante = piece_identifier(partie->damier[deplacement->position_initial.x][deplacement->position_initial.y]);

	//Supprimer la piece jou�
	partie->damier[deplacement->position_initial.x][deplacement->position_initial.y] = '.';

	//Supprimer les pieces captur�es
	for (compteur = 0; compteur < deplacement->nombre_pieces_capturees; compteur++)
	{
		partie->damier[deplacement->pieces_capturees[compteur].pos.x][deplacement->pieces_capturees[compteur].pos.y] = '.';
	}

	//Si la piece a �t� promu
	if (deplacement->promotion)
	{
		//Une cr�e une nouvelle piece pour le m�me joueur mais promue
		modifier_case(partie, piece_creer(piece_joueur(piece_courante), true), deplacement->position_finale);
	}
	else {
		//On pose le m�me caract�re de la pi�ce jou�e
		modifier_case(partie, piece_courante, deplacement->position_finale);
	}

	//Ajouter le deplacement et la configuration dans les listes doublements chainn�es
	//Si il s'agit du premier deplacement : (les deux listes doivent contenir le m�me nombre d'element)
	if (partie->coups_joues.nombre_deplacements == 0)
	{
		//Pas de d�placement pr�cedent car c'est le premier
		deplacement->ePrecedent = NULL;
		configuration->ePrecedent = NULL;

		//mettre le d�placement comme element de 'd�but' et 'finale' de la liste
		partie->coups_joues.eDebut = deplacement;
		partie->coups_joues.eFinal = deplacement;

		//mettre la configuration comme element de 'd�but' et 'finale' de la liste
		partie->configuration.eDebut = configuration;
		partie->configuration.eFinal = configuration;
	}
	else {
		//Enregistrer le d�placement et la configuration pr�cedents dans les �lements de d�placement courant
		deplacement->ePrecedent = partie->coups_joues.eFinal;
		configuration->ePrecedent = partie->configuration.eFinal;

		//Changer le d�placement suivant du d�placement pr�cedent
		partie->coups_joues.eFinal->eSuivant = deplacement;
		partie->configuration.eFinal->eSuivant = configuration;

		//mettre se deplacement comme finale
		partie->coups_joues.eFinal = deplacement;
		partie->configuration.eFinal = configuration;
	}

	//Le d�placement suivant n'est pas encore l�
	deplacement->eSuivant = NULL;
	configuration->eSuivant = NULL;

	//Augmenter le nombres de d�placements
	partie->coups_joues.nombre_deplacements++;
	partie->configuration.nombre_configuration++;
}

//Sauvegarder la partie et le plateaux actuelle
void partie_sauvegarder(struct Partie *partie, char *nom_fichier)
{
	char fichier_chemin_complet[MAX_BUFFER];
	int colonne_courante;
	int ligne_courante;

	struct Deplacement *deplacement;
	int dep_courant;
	FILE *fichier;

	//Nettoyer le nom de fichier, MAX_BUFFER est la taille standard des chaines
	nettoyer_chaine(nom_fichier, MAX_BUFFER);

	//Creer le fichier PLATEAU (.plt)
	sprintf(fichier_chemin_complet, "Plateaux/%s.plt", nom_fichier);
	fichier = fopen(fichier_chemin_complet, "w");

	if (fichier)
	{
		//Ecrire l'en-t�te
		fprintf(fichier, "PL\n");

		for (ligne_courante = 9; ligne_courante >= 0; ligne_courante--)
		{
			for (colonne_courante = 0; colonne_courante < 10; colonne_courante++)
			{
				fprintf(fichier, "%c", partie->damier[colonne_courante][ligne_courante]);
			}

			//Nouvelle ligne !
			fprintf(fichier, "\n");
		}

		//Fermer le fichier
		fclose(fichier);

		printf("\nLe fichier 'plateau' a ete sauvegarder avec succes\n");
		printf("FICHIER : %s\n", fichier_chemin_complet);
	}
	else {
		printf("\nERREUR   : La sauvegarde du fichier 'plateau' a echoue\n");
	}

	//Creer le fichier PARTIE (.part)
	sprintf(fichier_chemin_complet, "Parties/%s.part", nom_fichier);
	fichier = fopen(fichier_chemin_complet, "w");

	if (fichier)
	{
		//Ecrire l'en-t�te
		fprintf(fichier, "PR\n");

		deplacement = partie->coups_joues.eDebut;

		for (ligne_courante = 0; ligne_courante < partie->coups_joues.nombre_deplacements; ligne_courante++)
		{
			//Ecrire le nombre des positions intermediaires, ls dep. initiale et finale sont inclus)
			fprintf(fichier, "%i ", deplacement->nombre_positions_intermediares + 2);

			//Ecrire les coords de la piece � jouer (position initiale)
			fprintf(fichier, "%i%i ", deplacement->position_initial.x, deplacement->position_initial.y);

			//Ecrire les coords des positions interm�diaire
			for (dep_courant = 0; dep_courant < deplacement->nombre_positions_intermediares; dep_courant++)
			{
				fprintf(fichier, "%i%i ", deplacement->positions_intermediaires[dep_courant].x,
					deplacement->positions_intermediaires[dep_courant].y);

			}

			//Ecrire les coords de la position finale
			fprintf(fichier, "%i%i\n", deplacement->position_finale.x, deplacement->position_finale.y);

			//Pointer vers le deplacement suivant
			deplacement = deplacement->eSuivant;
		}

		//Fermer le fichier
		fclose(fichier);

		printf("\nLe fichier 'partie' a ete sauvegarder avec succes\n");
		printf("FICHIER : %s\n", fichier_chemin_complet);
	}
	else {
		printf("\nERREUR   : La sauvegarde du fichier 'partie' a echoue\n");
	}
}

//Annuler le dernier mouvement
void annuler_mouvement(struct Partie *partie)
{
	//D�claration des variables
	struct Deplacement *deplacement;	//Poniteur vers le deplacement dernier dans la liste des coups jou�s
	struct Configuration *configuration;

	//Initialisation
	deplacement = partie->coups_joues.eFinal;
	configuration = partie->configuration.eFinal;

	//Charger le demier precedent qui enregistr� dans la liste des configurations
	memcpy(partie->damier, configuration->damier, sizeof(char) * 10 * 10);

	//changer le joueur qui � le trait
	changer_joueur(partie);

	//Liberer la m�moire des listes pour les position intermediaire et piece captur�
	free(deplacement->positions_intermediaires);
	free(deplacement->pieces_capturees);

	//Pas de deplacement pr�cedent � celui ci
	if (deplacement->ePrecedent == NULL)
	{
		partie->coups_joues.eDebut = NULL;
		partie->coups_joues.eFinal = NULL;

		partie->configuration.eDebut = NULL;
		partie->configuration.eFinal = NULL;

		free(deplacement);
		free(configuration);
	}
	else {
		//Pointer vers le deplacement precedent � celui ci
		deplacement = deplacement->ePrecedent;
		configuration = configuration->ePrecedent;

		//Liberer la m�moire du d�placement annul�
		free(deplacement->eSuivant);
		free(configuration->eSuivant);

		//Mettre � jour des listes
		deplacement->eSuivant = NULL;
		configuration->eSuivant = NULL;

		partie->coups_joues.eFinal = deplacement;
		partie->configuration.eFinal = configuration;
	}

	//D�duire le deplacement annul�
	partie->coups_joues.nombre_deplacements--;
	partie->configuration.nombre_configuration--;
}

//Detruire la partie
void partie_detruire(struct Partie *partie)
{
	struct Deplacement *deplacement;
	struct Configuration *configuration;

	if (partie == NULL)
	{
		//Rien � d�truire alors retour
		return;
	}

	//Liberer la m�moire des �l�ment des coups jou�s
	//On commence par l'element finale
	deplacement = partie->coups_joues.eFinal;
	configuration = partie->configuration.eFinal;

	/*
	Le nombre d'elements dans les deux liste est identique,
	il suffit de prendre une seule variable qui represente
	le nombre maximum des elements dans les deux listes
	*/
	while (partie->coups_joues.nombre_deplacements != 0)
	{
		//Liberer les listes de deplacements intermediaires et pieces captur�s
		free(deplacement->positions_intermediaires);
		free(deplacement->pieces_capturees);

		//Liberer les listes de configuration

		//Si nous sommes arriv�s au premier element
		if (deplacement->ePrecedent == NULL)
		{
			//Liberer les derniers elements dans les listes doublements chainn�es
			free(deplacement);
			free(configuration);

			//Sortir du boucle
			break;
		}
		else {
			//On prend l'element precedent de l'element courant
			deplacement = deplacement->ePrecedent;
			configuration = configuration->ePrecedent;

			//Liberer l'element suivant de l'element actuel
			free(deplacement->eSuivant);
			free(configuration->eSuivant);
		}

		//d�cr�menter le deplacement et la configuration liber�
		partie->coups_joues.nombre_deplacements--;
		partie->configuration.nombre_configuration--;
	}

	//Liberer partie apr�s avoir liberer les listes doublements chainn�es
	free(partie);
}

//Changer le joueur qui a le trait
void changer_joueur(struct Partie *partie)
{
	//Changer le joueur qui a le trait en inversant la valeur du boolean
	partie->joueur = !partie->joueur;
}

//Jouer la partie
void partie_jouer(struct Partie *partie)
{
	//Declarer les variables n�cessaires 
	struct Deplacement *deplacement;
	struct Resultat_saisie Saisie;

	bool partie_termine;

	//Initialiser les variables
	partie_termine = false;

	while (!partie_termine)
	{
		//Clear screen
		effacer_ecran();

		//Afficher le plateau
		afficher_plateau(partie, NULL);

		//Questionner le joueur et attendre la r�ponse
		Saisie = saisie_case(partie);

		//Si une commande est �crite 
		if (Saisie.commande_chaine != NULL)
		{
			//En cas de commande 'fermer'
			if (strcmp(Saisie.commande_chaine, "fermer") == 0)
			{
				//Commande valide : Fermer
				Saisie.commande_valide = true;

				//Proposer la sauvegarder
				if (proposer_sauvegarde(partie))
				{
					//Terminer la partie si la fonction renvoie VRAI
					partie_termine = true;
				}
			}

			//En cas de commande 'annuler'
			if (strcmp(Saisie.commande_chaine, "annuler") == 0)
			{
				if (partie->coups_joues.nombre_deplacements <= 0)
				{
					printf("ERREUR   : La liste des coups joues est vide !");
					attendre();
					continue;
				}

				//On annule le deplacement
				annuler_mouvement(partie);
				continue;
			}

			//Liberer la memoire allouer pour l'enregistrement de la commande
			free(Saisie.commande_chaine);

			//Si la commande est valide on reprend la boucle
			if (Saisie.commande_valide)
				continue;
		}

		//Informer l'utilisateur si la commande n'est pas valide
		if (!Saisie.commande_valide)
		{
			printf("ERREUR   : Commande non valide, reessayez...");
			attendre();
			continue;
		}

		//Allou� la m�moire pour le d�placement
		deplacement = (struct Deplacement*) malloc(sizeof(struct Deplacement));

		//Verifier les mouvements obtenus dont la premiere coordonn�es est la position initiale de la piece
		if (deplacement_valide(partie, Saisie.liste_coordonnees, Saisie.nombre_coordonnees, deplacement) == true)
		{
			//Modifier le damier selon les deplacements
			modif_damier(partie, deplacement);

			if (partie_terminee(partie))
			{
				printf("\n********** PARTIE TERMINEE **********\n");
				printf("Le joueur %d a gagne ce match !\n", partie->joueur + 1);

				//Proposer la sauvegarde de la partie
				proposer_sauvegarde(partie);

				//Fin de la partie
				partie_termine = true;
				continue;
			}

			//changer le joueur qui a le trait
			changer_joueur(partie);
		}
		else {
			//Liberer la memoire pour le deplacement echou�
			free(deplacement);

			printf("ERREUR   : Deplacement non valide, reessayez...");
			attendre();
			continue;
		}

		//Lib�rer la m�moire allou�e pour la liste des mouvements
		free(Saisie.liste_coordonnees);
	}
}

//Jouer la replay/partie charger
void replay_jouer(struct Partie *partie)
{
	//D�claration des variables
	bool replay_termine = false;
	char commande[MAX_BUFFER];
	struct Configuration *configuration;
	int compteur;

	//reinitialiser le compteur du replay
	configuration = partie->configuration.eDebut;
	compteur = 0;

	//On avance deplacement par deplacement
	while (!replay_termine)
	{
		//Effacer l'ecran
		effacer_ecran();

		//Afficher le plateau
		afficher_plateau(partie, configuration);

		//Afficher quelques informations
		printf("REPLAY : Deplacements : %i / %i\n\n", compteur, partie->configuration.nombre_configuration);
		printf("[S]uivant / [P]recedent / [C]ontinuer la partie / [F]ermer  > ");

		//Lire la commande
		fgets(commande, sizeof(commande), stdin);

		switch (tolower(commande[0]))
		{
		case 's':
			if (compteur < partie->configuration.nombre_configuration)
			{
				/*
				Dans le dernier deplacement il faut affichier le damier final
				et cette derniere ne se trouve pas dans la liste de configuration
				mais la configuration->eSuivant est NULL alors afficher_plateaux
				va afficher le dernier status du damier !!!!!
				*/

				//Charger la configuration suivant
				configuration = configuration->eSuivant;

				//Incr�menter le compteur
				compteur++;
			}
			break;

		case 'p':
			if (compteur > 0)
			{
				if (configuration == NULL)
				{
					/*
					Si la configuration est null alors le dernier damier apres le dernier
					deplacement est afficher alors l'element precedent est la derniere
					entr�e dans la liste de configuration !!!
					*/
					configuration = partie->configuration.eFinal;
				}
				else {
					//Charger la configuration pr�cedente
					configuration = configuration->ePrecedent;
				}

				//D�cr�menter le compteur
				compteur--;
			}
			break;

		case 'c':
			//Continuer � jouer la partie
			partie_jouer(partie);

			//Terminer la partie
			replay_termine = true;
			break;

		case 'f':

			//Terminer la partie
			replay_termine = true;
			break;

		default:
			break;
		}
	}
}

//Tester si les deplacements entr�s par le joueur sont valides
bool deplacement_valide(struct Partie *partie, struct coord deplacements[], int n_deplacements, struct Deplacement *dep)
{
	/*
	struct Deplacement *dep:

	- La liste de d�placements doit contenir au moins 2 coordonn�es initiale et finale.
	- La premi�re entr�e dans la liste des deplacements est la position initiale de la pi�ce jou�e.
	*/

	//Declaration des variables n�cessaire pour le processus de v�rification3
	struct Piece piece_courante;		//La piece du joueur
	struct Piece piece_prise;			//La ou les piece(s) qui seront captur�e(s)
	struct coord direction;				//Indique la direction du mouvement
	struct coord case_suivante;			//Enregistre la case qui est entrain d'�tre tester 
	int coord_courant;					//Le deplacement courant dans la liste de deplacements
	int compteur;						//Un compteur temporaire

										//Les booleans de verification des mouvement de dammes
	bool piece_adversaire_trouve;

	//La list des coord des pieces adversaires captur�es
	struct Piece_capture liste_pieces_capturees[MAX_CAPTURES];

	//La list des coord pour les d�placements interm�diaires
	struct coord liste_dep_intermediaire[MAX_DEPLACEMENT];

	//Lire le caract�re trouv�s dans la position initiale dans le damier 
	piece_courante = piece_identifier(partie->damier[deplacements[0].x][deplacements[0].y]);

	//Verifier si une piece existe vraiment dans la position initiale
	if (piece_joueur(piece_courante) == -1) { return false; }

	//V�rifier si la pi�ce correspond au joueur qui a le trait
	if (!partie->joueur && piece_joueur(piece_courante) != 0) { return false; }
	if (partie->joueur && piece_joueur(piece_courante) != 1) { return false; }

	//Initialiser la structure du deplacement
	dep->nombre_positions_intermediares = 0;
	dep->positions_intermediaires = NULL;
	dep->nombre_pieces_capturees = 0;
	dep->pieces_capturees = NULL;
	dep->promotion = false;

	//Initialisation des boolean de verification
	piece_adversaire_trouve = false;

	//Enregistrer la position initiale et finale
	dep->position_initial = deplacements[0];
	dep->position_finale = deplacements[n_deplacements - 1];

	//Verifier si la pi�ce est une damme
	if (piece_courante.status)
	{
		/*
		- En cas d'une pi�ce promue
		-----------------------------------------------------------------------------------------------
		- Verifier si c'est un d�placement ou prise cela est fait en analysant chaque case
		Entre la position intiale et la position finale, si aucune piece adversaire n'est detect�
		cela est deplacement et pas de deplacements intermediaires allou�s, sinon il s'agit d'une prise
		alors entre chaque deplacement intermediaire il faut trouv� une et seulement pi�ce adversaire !!

		*/

		for (coord_courant = 0; coord_courant < (n_deplacements - 1); coord_courant++)
		{
			//Degager la direction du mouvement de la piece
			direction = direction_du_mouvement(deplacements[coord_courant], deplacements[coord_courant + 1]);

			//Le deplacement de la damme doit �tre diagonale
			if (abs(direction.x) != abs(direction.y))
				return false;

			//Initialiser 'case_suivante'
			case_suivante = deplacements[coord_courant];

			//Pour chaque case entre l'intiale et finale
			while (case_suivante.x != deplacements[coord_courant + 1].x &&
				case_suivante.y != deplacements[coord_courant + 1].y)
			{
				//Case suivante
				case_suivante.x += direction.x;
				case_suivante.y += direction.y;

				if (case_vide(partie, case_suivante))
				{
					//Case vide alors pas de probleme
					continue;
				}
				else
				{
					//Identifier la piece
					piece_prise = piece_identifier(partie->damier[case_suivante.x][case_suivante.y]);

					if (piece_joueur(piece_prise) != piece_joueur(piece_courante))
					{
						if (piece_adversaire_trouve)
						{
							//Si on a d�ja trouv� une autre piece adversaire cela dit que se d�placement est invalide
							return false;
						}
						else
						{
							//Verifie que cette piece n'a pa �t� marqu� pour prise
							for (compteur = 0; compteur < dep->nombre_pieces_capturees; compteur++)
							{
								if (liste_pieces_capturees[compteur].pos.x == case_suivante.x &&
									liste_pieces_capturees[compteur].pos.y == case_suivante.y)
								{
									//piece d�ja marqu�e pour �tre captur�es on ne la saute pas une autre fois
									return false;
								}
							}

							//Enregistrer la position de la pi�ce captur�e
							liste_pieces_capturees[dep->nombre_pieces_capturees].pos = case_suivante;
							liste_pieces_capturees[dep->nombre_pieces_capturees].status = piece_prise.status;
							dep->nombre_pieces_capturees++;

							//une piece adverdaire a �t� trouv�
							piece_adversaire_trouve = true;

							continue;
						}
					}
					else
					{
						if (case_suivante.x == deplacements[0].x && case_suivante.y == deplacements[0].y)
						{
							/*
							EXCEPTION :
							Si la pi�ce trouv� est la m�me pi�ce jou�e : Dans un rafle on deplace
							la piece CAD la case � la position initiale est vide pour le moment
							Alors on peut s'arr�ter sur la case de d�part
							*/
							continue;
						}
						else {
							//Piece du m�me joueur ou autre obstacle : mouvement invalide
							return false;
						}
					}
				}
			}

			if (!piece_adversaire_trouve)
			{
				//Si aucune piece adversaire n'a �t� trouv� entre les deux positions alors c'est un deplacement normale
				dep->position_finale = deplacements[1];

				//Si c'est juste un avancement mais le joueur a entr� plus que 2 coordonn�es
				if (n_deplacements > 2)
				{
					return false;
				}

				return true;
			}
			else {
				if (coord_courant > 0)
				{
					//Sinon enregistrer la position interm�diaire si cette position n'est pas l'initial
					liste_dep_intermediaire[dep->nombre_positions_intermediares] = deplacements[coord_courant];
					dep->nombre_positions_intermediares++;
				}

				//Reinitialiser 'piece_adversaire_trouve' pour le deplacement intermediaire suivant
				piece_adversaire_trouve = false;
			}
		}

	}
	else {

		/*
		- En cas d'une pi�ce non promue
		-----------------------------------------------------------------------------------------------
		- Verifier si le premier d�placement est un avancement ou une prise.
		- En cas d'un avancement la position X et Y doivent �tre differentes de l'intiale de �1.

		- En cas d'une prise les positions X et Y doivent �tre differentes
		de l'intiale de �2 uniquement dans chaque deplacement(non promu).
		*/

		if ((abs(deplacements[0].x - deplacements[1].x) == 1) &&
			(abs(deplacements[0].y - deplacements[1].y) == 1))
		{
			//Degager la direction du deplacement
			direction = direction_du_mouvement(deplacements[0], deplacements[1]);

			//Verifier si l'avancement est toujours vers le cot� de l'adversaire
			if ((piece_joueur(piece_courante) == 0 && direction.y == 1) ||
				(piece_joueur(piece_courante) == 1 && direction.y == -1))
			{
				//Verifier si la case est vide
				if (case_vide(partie, deplacements[1]))
				{
					//Mettre � jour la position finale car c'est juste un avancement
					dep->position_finale = deplacements[1];

					//Si c'est juste un avancement mais le joueur a entr� plus que 2 coordonn�es
					if (n_deplacements > 2)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				//Avancement non valide (avancement vers l'arri�re, droite ou gauche)
				return false;
			}
		}
		else
		{
			//Verifier si la liste des deplacements de prises est valide
			for (coord_courant = 0; coord_courant < (n_deplacements - 1); coord_courant++)
			{
				if ((abs(deplacements[coord_courant].x - deplacements[coord_courant + 1].x) == 2) &&
					(abs(deplacements[coord_courant].y - deplacements[coord_courant + 1].y) == 2))
				{
					//Degager la direction du mouvement de la piece
					direction = direction_du_mouvement(deplacements[coord_courant], deplacements[coord_courant + 1]);

					//Les coords de la pi�ce � prendre
					case_suivante.x = deplacements[coord_courant].x + direction.x;
					case_suivante.y = deplacements[coord_courant].y + direction.y;

					//Verifier si la case suivante est vide
					if (!case_vide(partie, deplacements[coord_courant + 1]))
					{
						/*
						EXCEPTION :
						Si la pi�ce trouv� est la m�me pi�ce jou�e : Dans un rafle on deplace
						la piece CAD la case � la position initiale est vide pour le moment
						Alors on peut s'arr�ter sur la case de d�part
						*/
						if (deplacements[coord_courant + 1].x != deplacements[0].x ||
							deplacements[coord_courant + 1].y != deplacements[0].y)
						{
							//Piece du m�me joueur ou autre obstacle : mouvement invalide
							return false;
						}
					}

					//Verifier si la piece captur� est une piece valide de l'adversaire
					piece_prise = piece_identifier(partie->damier[case_suivante.x][case_suivante.y]);

					if (piece_joueur(piece_prise) != -1 && piece_joueur(piece_prise) != piece_joueur(piece_courante))
					{
						//Verifie que cette piece n'a pa �t� marqu� pour prise
						for (compteur = 0; compteur < dep->nombre_pieces_capturees; compteur++)
						{
							if (liste_pieces_capturees[compteur].pos.x == case_suivante.x &&
								liste_pieces_capturees[compteur].pos.y == case_suivante.y)
							{
								//piece d�ja marqu�e pour �tre captur�es on ne la saute pas une autre fois
								return false;
							}
						}

						//Enregistrer la position de la pi�ce captur�e
						liste_pieces_capturees[dep->nombre_pieces_capturees].pos = case_suivante;
						liste_pieces_capturees[dep->nombre_pieces_capturees].status = piece_prise.status;
						dep->nombre_pieces_capturees++;

					}
					else {
						//Pi�ce invalide
						return false;
					}

					//Mouvement valide, on enregistre les coordon�es intermediaire
					if (coord_courant > 0 && coord_courant < (n_deplacements - 1))
					{
						//Enregistrer la position interm�diaire
						liste_dep_intermediaire[dep->nombre_positions_intermediares] = deplacements[coord_courant];
						dep->nombre_positions_intermediares++;
					}
				}
				else
				{
					//Les coordonnees du mouvement ne pr�sentent pas une prise
					return false;
				}
			}
		}
	}

	/*
	Les deplacements et prises valides, alors on copie la liste des pos. intermediares et pieces captur�es
	dans la structure deplacement qui sera ajouter � la liste doublement chain�e 'coups_jou�s' dans la partie
	*/

	//Verifier si c'est une promotion
	if ((piece_joueur(piece_courante) == 0 && !piece_courante.status && dep->position_finale.y == 9) ||
		(piece_joueur(piece_courante) == 1 && !piece_courante.status && dep->position_finale.y == 0))
	{
		//marquer la promotion dans le deplacement
		dep->promotion = true;
	}

	//Allocation de la m�moire n�cessaire pour les listes
	dep->positions_intermediaires = (struct coord*)malloc(dep->nombre_positions_intermediares * sizeof(struct coord));
	memcpy(dep->positions_intermediaires, liste_dep_intermediaire, dep->nombre_positions_intermediares * sizeof(struct coord));

	dep->pieces_capturees = (struct Piece_capture*)malloc(dep->nombre_pieces_capturees * sizeof(struct Piece_capture));
	memcpy(dep->pieces_capturees, liste_pieces_capturees, dep->nombre_pieces_capturees * sizeof(struct Piece_capture));

	return true;
}

//Tester si une case su damier est vide
bool case_vide(struct Partie *p, struct coord pos)
{
	if (p->damier[pos.x][pos.y] == '.')
		return true;
	else
		return false;
}

//Proposer la sauvegarde
bool proposer_sauvegarde(struct Partie *partie)
{
	char Reponse[MAX_BUFFER];

	//Si le joueur ferme sans avoir jouer une fois
	if (partie->coups_joues.nombre_deplacements == 0)
	{
		printf("\nRien a sauvegarder, aucun deplacement detecte !\n");
		return true;
	}

	//Si le plateau a �t� charg� d'apres un fichier alors les coups jou�s ne commence pas d'une nouvelle partie
	//CAD on ne peut pas sauvegarder la partie/replay '.part'
	if (partie->plateau_charge)
	{
		printf("\nLe plateau a ete charger d'apres un fichier 'plateau'!\nLa sauvegarde du fichier 'partie' est impossible.\n");
		return true;
	}

	//Questioner l'utilisateur � propos de la sauveguard
	printf("\nVoulez-vous sauvegarder cette partie ? [O]ui / [N]on / [A]nnuler\n>");

	//Lire la reponse de l'utilisateur
	fgets(Reponse, sizeof(Reponse), stdin);

	switch (tolower(Reponse[0]))
	{
	case 'o':
		printf("\nVeuillez inserer le nom du fichier : \n>");

		//Lire la reponse de l'utilisateur
		fgets(Reponse, sizeof(Reponse), stdin);

		//Enregistrer la partie
		partie_sauvegarder(partie, Reponse);

		return true;

	case 'n':
		return true;

	default:
		return false;
	}
}

//Tester si la partie a termin�
bool partie_terminee(struct Partie *partie)
{
	struct Piece piece_courante;
	int nombre_piece_joueur_1,
		nombre_piece_joueur_2,
		colonne_courante,
		ligne_courante;

	//Initialiser
	nombre_piece_joueur_1 = 0;
	nombre_piece_joueur_2 = 0;

	for (ligne_courante = 0; ligne_courante < 10; ligne_courante++)
	{
		for (colonne_courante = 0; colonne_courante < 10; colonne_courante++)
		{
			piece_courante = piece_identifier(partie->damier[ligne_courante][colonne_courante]);

			if (piece_courante.joueur == 0)
			{
				nombre_piece_joueur_1++;
			}
			else if (piece_courante.joueur == 1)
			{
				nombre_piece_joueur_2++;
			}
		}
	}

	if (nombre_piece_joueur_1 == 0 || nombre_piece_joueur_2 == 0)
	{
		return true;
	}

	return false;
}