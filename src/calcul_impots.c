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
//CLASSE  CALCUL - PERMET DE CALCULER EN FONCTION DES FAIT ET DES REGLES LES IMPOTS

//On décrypte la signification des deductions
double calculImpot(double revenu, ST_CONTENT *deduc){
	double aPayer=0;
	int i = 0;
	i = decryptCase(deduc);
	//par mois
	revenu /=12;
	//on a quatre reductions
	if(i>=14){
		aPayer = 0.55*revenu;
	//on a trois reductions
	}else if (i>9){
		aPayer = 0.65*revenu;
	//on a deux reductions
	}else if (i>3){
		aPayer = 0.75*revenu;
	//on a une reduction
	}else if(i>=0){
		aPayer = 0.85*revenu;
	}else{
		aPayer = revenu;
	}
	return aPayer;
}
//on recherche dans quel cas on est selon les deductions
//on pars du plus grand ensemble de deduction au plus petit
//car le plus grand englobe forcement les plus petits
//97 -> a
//111 -> o
int decryptCase(ST_CONTENT *deduc){
	int i = 0;
	ST_CONTENT *tp = NULL;
	for(i = 14;i>=0;i--){
		tp = deduc;
		while(tp!=NULL){
			if(tp->name == (i+97)){
				return i;
			}
			tp = tp->suiv;
		}
	}
	return -1;
}
//Mode : 1 - Avant/2 - Arriere/3 - Les deux + details
void checkSituation(double* solde,int mode){
	double aPayer = 0;
	ST_CONTENT *teteF = NULL;
	ST_REGLES *teteR = NULL;
	ST_CONTENT *teF = NULL;
	ST_CONTENT *teteD = NULL;
	ST_CONTENT *teteDD = NULL;
	//temporaire
	ST_CONTENT *teD = NULL;
	ST_CONTENT *teDD = NULL;
	char temp[1] = {0};
	//on demande de rentrer les faits a l'utilisateur
	do{
		printf("\nVeuillez indiquer vos revenus annuel en euros net :");
		viderBuffer();
		*solde = lireDouble();
	}while(*solde==0);
	printf("\n------------->Revenus : %f",*solde);
	do{
		printf("\nAvez vous des enfants a charge? (oui tapez O, non tapez N) :");
	}while(lire(temp,1)==0);
	if(strcmp(temp,"O")==0){
		newContent('E',1,&teteF);
	}
	do{
		printf("\nAvez vous des personnes agees a charge? (oui tapez O, non tapez N) :");
	}while(lire(temp,1)==0);
	if(strcmp(temp,"O")==0){
		newContent('A',1,&teteF);
	}
	do{
		printf("\nPayez vous des pensions ? (oui tapez O, non tapez N) :");
	}while(lire(temp,1)==0);
	if(strcmp(temp,"O")==0){
		newContent('P',1,&teteF);
	}
	do{
		printf("\nAvez vous des aides de la CAF ? (oui tapez O, non tapez N) :");
	}while(lire(temp,1)==0);
	if(strcmp(temp,"O")==0){
		newContent('C',1,&teteF);
	}
	//plus de details si mode 3
	if(mode == 3){
		teF = teteF;
		while(teF!=NULL){
			printf("\nPour le fait : %c la valeur vaut : %d",teF->name,teF->value);
			teF = teF->suiv;
		}
		printf("\n");
	}
	teteR = openRulesFile();
	//erreur fichier on free et on quitte
	if(teteR == NULL){
		teteF = freeContent(teteF);
		return;
	}
	//APPEL CHAINAGE AVANT
	if(mode == 1 || mode == 3){
		chainageAvant(&teteD,teteF,teteR);
		teD = teteD;
		while(teD!=NULL){
			//plus de details
			if(mode == 3){
				printf("\nDeductions AV: %c",teD->name);
			}
			teD = teD->suiv;
		}
		printf("\n");
	}
	//APPEL CHAINAGE ARRIERE
	if(mode == 2 || mode == 3){
		chainageArriere(&teteDD,teteF,teteR);
		teDD = teteDD;
		while(teDD!=NULL){
			//plus de details
			if(mode == 3){
				printf("\nDeductions AR: %c",teDD->name);
			}
			teDD = teDD->suiv;
		}
		printf("\n");
	}
	//Vrai calcul du contexte AVANT
	if(mode == 1 || mode == 3){
		aPayer = calculImpot(*solde,teteD);
		printf("\n\t{AV}[A PAYER] : Vous devez alors payer : %f euros pour %f euros de revenus\n",aPayer,*solde);
	}
	//Vrai calcul du contexte ARRIERE
	if(mode == 2 || mode == 3){
		aPayer = calculImpot(*solde,teteDD);
		printf("\n\t{AR}[A PAYER] : Vous devez alors payer : %f euros pour %f euros de revenus\n",aPayer,*solde);
	}
	//FREE
	teteDD = freeContent(teteDD);
	teteD = freeContent(teteD);
	teteR = freeRegles(teteR);
	teteF = freeContent(teteF);
	//retour au menu
	printf("\t->Appuyer sur 'entree' pour revenir au menu\n");
	getchar();
}

//MODE TEST - Appel du chainage avant avec les faits des tests
void modeTestChainageAvant(ST_CONTENT *teteF){
	ST_REGLES *teteR = NULL;
	ST_CONTENT *teteD = NULL;
	ST_CONTENT *teF = NULL;
	teteR = openRulesFile();
	//erreur fichier on free et on quitte
	if(teteR == NULL){
		teteF = freeContent(teteF);
		return;
	}
	teF = teteF;
	//on affiche les faits
	printf("\nFAIT : \n");
	while(teF!=NULL){
		printf("\t%c\n",teF->name);
		teF = teF->suiv;
	}
	//appel chainage avant
	chainageAvant(&teteD,teteF,teteR);
	printf("\nDEDUCTION : \n");
	while(teteD!=NULL){
		printf("\t%c\n",teteD->name);
		teteD = teteD->suiv;
	}
	teteD = freeContent(teteD);
	teteR = freeRegles(teteR);
	//retour au menu
	viderBuffer();
	printf("\t->Appuyer sur 'entree' pour revenir au menu\n");
	getchar();
}
//on charge le fichier de regles
//et on l'affiche simplement
void modeTestAfficherRegles(){
	ST_REGLES *teteR = NULL;
	teteR = openRulesFile();
	//erreur fichier on free et on quitte
	if(teteR == NULL){
		return;
	}
	//affichage
	printf("\nREGLES : \n");
	while(teteR!=NULL){
		printf("\t%s\n",teteR->regles);
		printf("\t\tFaits : %s -> Deduc : %c\n",teteR->faits,teteR->deduc);
		teteR = teteR->suiv;
	}
	//free
	teteR = freeRegles(teteR);
	//retour au menu
	viderBuffer();
	printf("\t->Appuyer sur 'entree' pour revenir au menu\n");
	getchar();
}

//MODE TEST - Appel du chainage arriere avec les faits des tests
void modeTestChainageArriere(ST_CONTENT *teteF){
	ST_REGLES *teteR = NULL;
	ST_CONTENT *teteD = NULL;
	ST_CONTENT *teF = NULL;
	teteR = openRulesFile();
	//erreur fichier on free et on quitte
	if(teteR == NULL){
		teteF = freeContent(teteF);
		return;
	}
	teF = teteF;
	printf("\nFAIT : \n");
	while(teF!=NULL){
		printf("\t%c\n",teF->name);
		teF = teF->suiv;
	}
	chainageArriere(&teteD,teteF,teteR);
	printf("\nDEDUCTION : \n");
	while(teteD!=NULL){
		printf("\t%c\n",teteD->name);
		teteD = teteD->suiv;
	}
	teteD = freeContent(teteD);
	teteR = freeRegles(teteR);
	//retour au menu
	viderBuffer();
	printf("\t->Appuyer sur 'entree' pour revenir au menu\n");
	getchar();
}