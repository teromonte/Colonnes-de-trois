#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../header/protocolColonne.h"

/*
 * Structures demande de partie
 */
typedef struct
{
  TPion color;
  TCoup typeMove;
  TCase placeMove;
  TDeplPion displaceMove;
} responseAI;

// request play
int requestAI(TPion color, int sockAI, responseAI *res);

/*
  output:
    TCoup ->   POS_PION, DEPL_PION, PASSE
    option:
      TCase = TCol + TLigne -> (  UN, DEUX, TROIS) + (  A, B, C)
      TDeplPion = TCase + TCase -> [(UN,DEUX,TROIS)+(A,B,C)]+[(UN,DEUX,TROIS)+(A,B,C)]

  input:
    TPion -> BLANC, NOIR
*/