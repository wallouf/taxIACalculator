#include "./outils_struc_impots.h"
void chainageAvant(ST_CONTENT** deduc,ST_CONTENT* fait,ST_REGLES* regles);
void chainageArriere(ST_CONTENT** deduc,ST_CONTENT* fait,ST_REGLES* regles);
int recurArriere(char aTrouver,ST_REGLES* regles,ST_CONTENT** deduc);
int GetNumberOfEvents(ST_REGLES* regles);
int verifDeductionDoesntExist(ST_CONTENT* verifDeduc,char Deduc);
void loadDeductionWithFait(ST_CONTENT** deduc,ST_CONTENT* fait);