/*
Calculateur d'impots
LOPEZ Xavier - TASSIN Christian
EMSE - P20 - ITII
*/
//BIBLIOTHEQUES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/calcul_impots.h"
#include "../inc/moteur_impots.h"
#include "../inc/outils_struc_impots.h"

// AIDE : AUTORISATION : chmod u+x fichier.sh
// AIDE : COMPILATION : gcc -Wall -o nom_du_fichier_objet  -c nom_du_fichier_source_C
// AIDE : EXECUTION : ./nom_du_fichier_objet

//CLASSE PRINCIPALE - APPEL DE FONCTIONS + AFFICHAGE DES MENUS
int menu();
int menuCalcul();
int modeDevMenu();

int main(void){
	//variables
	int choix = 0;
	int choixCal = 0;
	int choixDev = 0;
	double solde = 0.0;
	ST_CONTENT *teteF = NULL;
	do{
		//permet un affichage plus clair
		system("clear");
		//affichage menu principal
		choix = menu();
		system("clear");
		switch(choix){
			case 0:
				return 0;
			break;
			case 1:
				//MODE CALCUL
				system("clear");
				choixCal = menuCalcul();
				system("clear");
				if(choixCal>=1 && choixCal <=3){
					checkSituation(&solde,choixCal);
				}
			break;
			case 2:
				//MODE TEST EN BOUCLE
				do{
					system("clear");
					choixDev = modeDevMenu();
					system("clear");
					switch(choixDev){
						case 0:
						break;
						case 1:
							//faits depuis fichier
							teteF = freeContent(teteF);
							teteF = chargerFaitDepuisFichier();
						break;
						case 2:
							//faits depuis console
							teteF = freeContent(teteF);
							teteF = chargerFaitDepuisConsole();
						break;
						case 3:
							//avant
							if(teteF!=NULL){
								modeTestChainageAvant(teteF);
							}
						break;
						case 4:
							//arriere
							if(teteF!=NULL){
								modeTestChainageArriere(teteF);
							}
						break;
						case 5:
							//affichage regles
							modeTestAfficherRegles();
						break;
						default:
							choixDev=0;
						break;
					}
				}while(choixDev!=0);
				teteF = freeContent(teteF);
			break;
			default:
				printf("\nLe choix n'existe pas !\n");
				return 0;
			break;
		}
	}while(1==1);
	return 0;
}
//menu principal
int menu(){
	int choix = 0;
	printf("*************************************************");
	printf("\n***************CALCULATEUR D'IMPOT***************");
	printf("\n******************SUR LE REVENU******************");
	printf("\n*************************************************");
	printf("\n            1 - Calculer ses impots ?            ");
	printf("\n            2 - Tester le moteur ?               ");
	printf("\n            0 - Quitter ?                        ");
	printf("\n*************************************************");
	printf("\n*************************************************");
	printf("\n      Votre choix ? : ");
	scanf("%d",&choix);
	return choix;
}

//menu mode 1 calculer normalement
int menuCalcul(){
	int choix = 0;
	printf("*************************************************");
	printf("\n***************CALCULATEUR D'IMPOT***************");
	printf("\n******************SUR LE REVENU******************");
	printf("\n*************************************************");
	printf("\n       Mode de calcul ? :                        ");
	printf("\n            1 - Chainage avant ?                 ");
	printf("\n            2 - Chainage arriere ?               ");
	printf("\n            3 - Chainage Av et Ar avec details ? ");
	printf("\n            0 - Quitter ?                        ");
	printf("\n*************************************************");
	printf("\n*************************************************");
	printf("\n      Votre choix ? : ");
	scanf("%d",&choix);
	return choix;
}

//menu mode test
int modeDevMenu(){
	int choix = 0;
	printf("********************MODE TEST********************");
	printf("\n*************************************************");
	printf("\n       Source des FAITS ? :                      ");
	printf("\n            1 - Depuis un fichier                ");
	printf("\n            2 - Depuis la console                ");
	printf("\n       Type de recherche ? :                     ");
	printf("\n            3 - Chainage Avant                   ");
	printf("\n            4 - Chainage Arriere                 ");
	printf("\n            5 - Afficher les regles              ");
	printf("\n       Arreter le mode test ? :                  ");
	printf("\n            0 - Retourner au menu                ");
	printf("\n*************************************************");
	printf("\n      Votre choix ? : ");
	scanf("%d",&choix);
	return choix;
}
