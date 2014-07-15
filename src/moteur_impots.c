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
//CLASSE  MOTEUR - Contient le chainage avant et arriere

void chainageAvant(ST_CONTENT** deduc,ST_CONTENT* fait,ST_REGLES* regles){
	int find = 0;
	int size = 0;
	int reg	= 0;
	int i = 0;
	//variables temporaire pour se deplacer sans perdre la tête
	ST_REGLES* rules = NULL;
	ST_CONTENT* tpDeduc = NULL;
	loadDeductionWithFait(deduc,fait);
	//on parcours tout tant qu'on déduit de nouveaux faits
	do{	
		rules = regles;
		find = 0;
		//on parcours toutes les regles
		while(rules!=NULL){
			size = GetNumberOfEvents(rules);
			int validFact = 0;
			//on parcours tout les fait requis
			for(i=0;i<size;i++){
				tpDeduc = *deduc;
				//on parcours tout les fait disponibles
				while(tpDeduc!=NULL){
					if (tpDeduc->name == rules->faits[i]){
						//on vérifi que la deduction n'existe pas
						//si elle existe on continu
						if(verifDeductionDoesntExist(*deduc,rules->deduc)==0){
							validFact++;
							break;
						}
					}
					tpDeduc = tpDeduc->suiv;
				}
			}
			//si on a trouve autant que requis
			//on rajoute le fait et on indique qu'on a trouve (find)
			if(size >0 && validFact == size ){
				newContent(rules->deduc,1,deduc);
				find++;
			}
			reg++;
			rules = rules->suiv;
		}
	}while(find>0);
}

//chainage arriere afin de verifier si tout les elements d'une regle sont la
void chainageArriere(ST_CONTENT** deduc,ST_CONTENT* fait,ST_REGLES* regles){
	int find = 0;
	int size = 0;
	//variables temporaire pour se deplacer sans perdre la tête
	ST_REGLES* rules = NULL;
	loadDeductionWithFait(deduc,fait);
	//on parcours tant qu'on deduit de nouveaux faits
	do{	
		rules = regles;
		find = 0;
		//on parcours toutes les regles
		while(rules!=NULL){
			size = GetNumberOfEvents(rules);
			//on verifi d'abord que la deduction n'est pas deja trouve
			if(verifDeductionDoesntExist(*deduc,rules->deduc) == 0){
				//si on a trouve toutes les dependances (appel recursif)
				if(recurArriere(rules->deduc,regles,deduc)==1){
					find++;
				}
			}
			rules = rules->suiv;
		}
	}while(find>0);
}

//fonction recursive du chainage arriere
int recurArriere(char aTrouver,ST_REGLES* regles,ST_CONTENT** deduc){
	int result = 1;
	int size = 0;
	int i = 0;
	//variables temporaire pour se deplacer sans perdre la tête
    ST_CONTENT* tpDeduc = NULL;
    ST_REGLES* tpR = NULL;
	tpDeduc = *deduc;
	//Si le a trouver fait parti des faits
	//on valide directement
	while(tpDeduc!=NULL){
		if(aTrouver==tpDeduc->name){
			return 1;
		}
		tpDeduc = tpDeduc->suiv;
	}
	//on cherche une regle qui a comme deduction 'aTrouver'
	tpR = regles;
	//on parcours toutes les regles
	while(tpR!=NULL){
		//si une une regle contient la deduction qu'on cherche
		//on vérifie par recursivitee que les faits de cette
		//regle sont valide
		if(tpR->deduc == aTrouver){
			//combien y a t'il de faits ?
			size = GetNumberOfEvents(tpR);
			//on parcours les faits necessaires
			//multiplication pour prendre en compte tout les faits
			for(i = 0;i<size;i++){
				result *= recurArriere(tpR->faits[i],regles,deduc);
			}
			//sortie du recursif, on remonte
			//si on a tout les faits necessaires
			if(result==1){
				//on ajoute directement le résultat
				if(verifDeductionDoesntExist(*deduc,tpR->deduc) == 0){
					newContent(aTrouver,1,deduc);
				}
				return result;
			}
		}
		tpR = tpR->suiv;
	}//on n'as pas trouvé si on arrive la
	return 0;
}


//on vérifi que la deduction n'existe pas
//RETURN 1 si exite
//RETURN 0 si n'existe pas
int verifDeductionDoesntExist(ST_CONTENT* verifDeduc,char Deduc){
	int verif =0;
	//on parcours toutes les deductions et on verifie que deduc 
	//n'est pas dedans
	while(verifDeduc!=NULL){
		if (verifDeduc->name == Deduc){
			verif++;
			break;
		}
		verifDeduc = verifDeduc->suiv;
	}
	return verif;
}
//on extrait le nombre de fait, les faits et la deduction
int GetNumberOfEvents(ST_REGLES* regles){
	int i =0;
	while(regles->faits[i]!='\0' && i<22){
		i++;
	}
	return i;
}
//on transfere les faits dans deduction afin d'alleger le traitement par
//la suite
void loadDeductionWithFait(ST_CONTENT** deduc,ST_CONTENT* fait){
	while(fait!=NULL){
		newContent(fait->name,fait->value,deduc);
		fait = fait->suiv;
	}
}