#include "ColonClAserv.h"
#include <stdio.h>



void  lireDemane(char* demande , char* nom) {
  printf("Donner une demande et votre nom : ");
  scanf(" %c %s", demande , nom);
  printf("votre nom est %s: " ,nom);
}

void lireDemaneCoup(char* demande){
printf("Donner une demande :  ");
  scanf(" %c ", demande );
}
