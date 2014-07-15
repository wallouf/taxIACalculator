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
//CLASSE  outils_struc - Contient tous les outils et les structures


//on recupere le texte du clavier
// 1 - OK
// 0 - ERROR
int lire(char *chaine, int longueur){
    char *positionEntree = NULL;
	//s'il n'est pas nul on remplace le retour chariot
    if (fgets(chaine, (longueur+1), stdin) != NULL){
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL){
            *positionEntree = '\0';
        }else{
            viderBuffer();
        }
		//Si on a un caractere vide -> erreur
		if(*chaine==' '){
			return 0;
		}else{
			return 1;
		}
    //sinon on vide quand meme le buffer et on renvoi 0 pour signaler le probleme
    }else{
        viderBuffer();
        return 0;
    }
}
//lecture d'un double
//0 - ERROR
double lireDouble(){
    char nombreTexte[100] = {0};
    if (lire(nombreTexte, 100))    {
        // Si lecture du texte ok, convertir le nombre en long et le retourner
        return strtod(nombreTexte, NULL);
    }else{
        // Si probl�me de lecture, renvoyer 0
        return 0;
    }
}
//permet de vider le buffer
void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF){
        c = getchar();
    }
}

//on cree un nouveau maillon ST_CONTENT
void newContent(char nom,int value, ST_CONTENT ** tete){
	ST_CONTENT *nouveau = NULL;
	//allocation memoire
	nouveau = (ST_CONTENT*) malloc(sizeof(ST_CONTENT));
	//on rempli les champs
	nouveau->name = nom;
	nouveau->value = value;
	//on insere en tete
	if (tete==NULL){
		nouveau->suiv=NULL;
		tete = &nouveau;
	} else {
		nouveau->suiv=*tete;
		*tete = nouveau;
	}
}

//on cree un nouveau maillon ST_REGLES
void nouvelleRegles(char nom[25],char faits[22],char deduc, ST_REGLES ** tete){
	ST_REGLES *nouveau = NULL;
	//allocation memoire
	nouveau = (ST_REGLES*) malloc(sizeof(ST_REGLES));
	//on rempli les champs
	strcpy(nouveau->regles,nom);
	strcpy(nouveau->faits,faits);
	nouveau->deduc = deduc;
	//on insere en tete
	if (tete==NULL){
		nouveau->suiv=NULL;
		tete = &nouveau;
	} else {
		nouveau->suiv=*tete;
		*tete = nouveau;
	}
}

//on demande a l'utilisateur de rentrer les faits qu'il veut pour
//le mode test du moteur
ST_CONTENT * chargerFaitDepuisConsole(){
	ST_CONTENT *fait = NULL;
	long nbFait;
	long i = 0;
	char temp;
	// on demande le nombre de faits
	printf("\n\tVeuillez entrer le nombre de fait a enregistrer : ");
	viderBuffer();
	nbFait = lireDouble();
	//pour le nombre de faits on cree des ST_CONTENT de ces faits rentre par l'utilisateur
	for(i = 0;i<nbFait;i++){
		printf("\nVeuillez entrer la lettre designant le fait n= %ld : ",i);
		temp = getc(stdin);
		viderBuffer();
		newContent(temp,1,&fait);
	}
	printf("[INFO] Enregistrement reussi !\n");
	//retour au menu
	printf("\t->Appuyer sur 'entree' pour revenir au menu");
	getchar();
	return fait;
}

//On charge depuis le fichier "fait.txt" dans bin les fait pour
//le mode test du moteur
ST_CONTENT * chargerFaitDepuisFichier(){
	ST_CONTENT *fait = NULL;
	FILE *fp;
	int temp = 0;
	char tp;
	int cp = 0;
	int nb = 0;
	//ouverture du fichier
	fp = fopen("faits.txt","r");
	if (fp == NULL){
		printf("Le fichier n'existe pas !Veuillez recommencer depuis le debut.\n");
		return NULL;
	}
	//tant qu'on est pas à la fin du fichier
	do{
		nb = 0;
		//si on est a la fin du fichier
		if(feof(fp)!=0){
			break;
		}
		//on enregistre par ligne le fait
		temp = fgetc(fp);
		tp = temp;
		newContent(tp,1,&fait);
		//on va jusqu'a la fin de la ligne ou du fichier
		while (feof(fp)==0 && temp != '\n' && temp != ';'){
			temp = fgetc(fp);
			nb++;
		}
		//si on est sur le ";" soit on saute la ligne soit c'est la fin du fichier
		if(temp == ';'){
			//on avance jusqu'a la fin de ligne ou la fin de fichier
			while (feof(fp)==0 && temp != '\n'){
				temp = fgetc(fp);
			}
		}
		cp++;
		//s'il y a encore une ligne
	}while(temp== '\n');
	fclose(fp);
	printf("[INFO] Chargement de %d faits reussi !\n",cp);
	viderBuffer();
	//retour au menu
	printf("\t->Appuyer sur 'entree' pour revenir au menu");
	getchar();
	return fait;
}
//ouverture des regles depuis le fichier
ST_REGLES * openRulesFile(){
	//variable temporaire pour se deplacer sans perdre la tete
	ST_REGLES *tete = NULL;
	FILE *fp;
	int temp = 0;
	char tp[25] = {0};
	char faitTp[22] = {0};
	char deduc;
	int caseFait = 0;
	int cp = 0;
	int again = 0;
	int deducNow = 3;
	int caseRegle = 0;
	//ouverture du fichier
	fp = fopen("regles.txt","r");
	if (fp == NULL){
		printf("Le fichier n'existe pas !Veuillez recommencer depuis le debut.\n");
		return NULL;
	}
	//tant qu'on est pas à la fin du fichier
	do{
		//si on est a la fin du fichier
		if(feof(fp)!=0){
			break;
		}
		temp = fgetc(fp);
		again=0;
		caseFait = 0;
		deducNow = 4;
		caseRegle = 0;
		//on parcours la ligne jusqu'a la fin du fichier ou le retour chariot
		while (feof(fp)==0 && temp != '\n' && temp != ';' && caseRegle<24){
			//si on arrive a la '->' on arrete d'enregistrer les faits necessaire pour la
			//regle et on passe a la suite pour enregistrer la deduction
			if(temp=='-'){
				again++;
				deducNow = 4;
				faitTp[caseFait] = '\0';
			//On enregistre tout les deux coups les faits tant que on arrive pas a '->'
			}else if(again<=0 && (caseRegle%2)==0){
				faitTp[caseFait] = temp;
				caseFait++;
			}
			//Si on a passe '->' alors on compte trois coups pour arriver
			// a la deduction
			if(again>=1){
				deducNow = deducNow-1;
				if(deducNow==0){
					deduc = temp;
				}
			}
			tp[caseRegle] = temp;
			temp = fgetc(fp);
			caseRegle++;
		}
		//si on est a ';' soit on passe a la ligne suivante soit on est a la fin du fichier
		if(temp == ';'){
			//on avance jusqu'a la fin de ligne ou la fin de fichier
			while (feof(fp)==0 && temp != '\n'){
				temp = fgetc(fp);
			}
		}
		//on indique la fin de la chaine
		tp[caseRegle] = '\0';
		nouvelleRegles(tp,faitTp,deduc,&tete);
		cp++;
		//s'il y a encore une ligne
	}while(temp== '\n');
	fclose(fp);
	return tete;
}
//free : liberation memoire ST_CONTENT
ST_CONTENT* freeContent(ST_CONTENT* content){
    if(content == NULL){
        return NULL;
    }else{
        ST_CONTENT* tmp;
        tmp = content->suiv;
        free(content);
        return freeContent(tmp);
    }
}
//free : liberation memoire ST_REGLES
ST_REGLES* freeRegles(ST_REGLES* regles){
    if(regles == NULL){
        return NULL;
    }else{
        ST_REGLES* tmp;
        tmp = regles->suiv;
        free(regles);
        return freeRegles(tmp);
    }
}
