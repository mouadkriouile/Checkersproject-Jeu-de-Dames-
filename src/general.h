#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Definition d'un boolean
#define true  1
#define false 0
typedef int bool;

#ifdef _WIN32
#define CLEAR "cls"
#pragma warning(disable : 4996)
#else //In any other OS (linux)
#define CLEAR "clear"
#endif

#define MAX_BUFFER 256
#define MAX_DEPLACEMENT 16
#define MAX_CAPTURES 16

//Structure qui represente une coordonnée
struct coord {
	int x;
	int y;
};

void attendre(void);
void effacer_ecran(void);

int caractere_valeur(char c);
int nettoyer_chaine(char *chaine, int taille);

struct coord coord_creer(int x, int y);
struct coord direction_du_mouvement(struct coord initial, struct coord finale);
