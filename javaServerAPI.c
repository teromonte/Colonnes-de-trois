#include "javaServerAPI.h"
#include <stdio.h>

void lireOperation(char* operateur, float* operande1, float* operande2) {
  printf("Donner expression (notation infixee : oprt opd1 opd2) : ");
  scanf(" %c %f %f", operateur, operande1, operande2);
}
    

