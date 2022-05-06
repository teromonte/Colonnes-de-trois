#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
/*
 * Structures demande de partie
 */
typedef struct
{
  int color;
  int type;
} Req;

// request play
int requestAI(int color, int sockAI);

/*
  output:
    TIdRequest -> PARTIE, COUP
    TCoup ->   POS_PION, DEPL_PION, PASSE
    option:
      TCase = TCol + TLigne -> (  UN, DEUX, TROIS) + (  A, B, C)
      TDeplPion = TCase + TCase -> [(UN,DEUX,TROIS)+(A,B,C)]+[(UN,DEUX,TROIS)+(A,B,C)]

  input:
    TPion -> BLANC, NOIR
*/