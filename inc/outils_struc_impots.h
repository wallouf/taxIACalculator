#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	typedef struct content
	{
		int value;		/*Nombre du fait*/
		char name;  /*Nom du fait*/
		struct content *suiv;     /*Pointeur liste fait*/
	}ST_CONTENT;

	typedef struct regles
	{
		char regles[25];  /*regle entiere*/
		char faits[22]; 		/*fait necessaire*/
		char deduc;			/*deduction de la regle*/
		struct regles *suiv;     /*Pointeur liste regles*/
	}ST_REGLES;
#endif
int lire(char *chaine, int longueur);
double lireDouble();
void viderBuffer();
void newContent(char nom,int value, ST_CONTENT ** tete);
void nouvelleRegles(char nom[25],char faits[22],char deduc, ST_REGLES ** tete);
ST_CONTENT * chargerFaitDepuisConsole();
ST_CONTENT * chargerFaitDepuisFichier();
ST_REGLES * openRulesFile();
ST_CONTENT* freeContent(ST_CONTENT* fait);
ST_REGLES* freeRegles(ST_REGLES* regles);