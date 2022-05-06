#ifndef _protocolJava_h
#define _protocolJava_h

#include <stdbool.h>

/*
 * Structures demande de partie
 */
typedef struct
{
  int color;
  int type;
} Req;

// Condensé des fonctions socket et connect
int playRequest();

/*
  TCoup ->   POS_PION, DEPL_PION, PASSE
  TCase = TCol + TLigne -> (  UN, DEUX, TROIS) + (  A, B, C)
  TPion -> BLANC, NOIR
  TIdRequest -> PARTIE, COUP
  TDeplPion = TCase + TCase -> [(UN,DEUX,TROIS)+(A,B,C)]+[(UN,DEUX,TROIS)+(A,B,C)]
*/

#endif
