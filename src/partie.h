#pragma once

#include "general.h"
#include "piece.h"
#include "saisie.h"

//Element de la liste des coups joués
typedef struct Deplacement
{
	struct coord position_initial;
	struct coord position_finale;

	struct coord *positions_intermediaires;
	struct Piece_capture *pieces_capturees;

	int	   nombre_positions_intermediares;
	int    nombre_pieces_capturees;

	bool promotion;

	struct Deplacement *eSuivant;
	struct Deplacement *ePrecedent;

} Deplacement;

//La structure qui represent la liste doublement chainnée des coups joués
typedef struct Liste_coups
{
	struct Deplacement *eDebut;
	struct Deplacement *eFinal;

	int nombre_deplacements;

} Liste_coups;

//Element de la liste des configuration
typedef struct Configuration
{
	char damier[10][10];

	struct Configuration *eSuivant;
	struct Configuration *ePrecedent;

} Configuration;

//La structure qui represent la liste doublement chainnée des configuration
typedef struct Liste_config
{
	struct Configuration *eDebut;
	struct Configuration *eFinal;

	int nombre_configuration;

} Liste_config;

//La structure qui represente une partie
typedef struct Partie
{
	char damier[10][10];					//Le Damier 10x10
	struct Liste_coups coups_joues;			//Liste doublement chainnée de coups joués
	struct Liste_config configuration;		//Liste doublement chainnée de configuration

	bool plateau_charge;					//Indique si le plateai a été charger à partir d'un fichier plateau
	bool joueur;							//Indique le joueur qui a le trait

} Partie;

/*
	Declaration des fonctions de gestion de partie
*/

struct Partie *partie_creer();
struct Partie *partie_nouvelle();
struct Partie *partie_charger(FILE *fichier);
struct Partie *replay_charger(FILE *fichier);

void afficher_plateau(struct Partie* partie, struct Configuration *config_a_afficher);
void modifier_case(struct Partie *partie, struct Piece piece, struct coord position);
void modif_damier(struct Partie *partie, struct Deplacement *deplacement);
void partie_sauvegarder(struct Partie *partie, char *nom_fichier);
void annuler_mouvement(struct Partie *partie);
void partie_detruire(struct Partie *partie);
void changer_joueur(struct Partie *partie);
void partie_jouer(struct Partie *partie);
void replay_jouer(struct Partie *partie);

bool deplacement_valide(struct Partie *partie, struct coord deplacements[], int n_deplacements, struct Deplacement *dep);
bool case_vide(struct Partie *p, struct coord pos);
bool proposer_sauvegarde(struct Partie *partie);
bool partie_terminee(struct Partie *partie);