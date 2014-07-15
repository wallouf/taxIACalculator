#include "./outils_struc_impots.h"
double calculImpot(double revenu, ST_CONTENT *deduc);
int decryptCase(ST_CONTENT *deduc);
void checkSituation(double* solde,int mode);
void modeTestAfficherRegles();
void modeTestChainageAvant(ST_CONTENT *teteF);
void modeTestChainageArriere(ST_CONTENT *teteF);