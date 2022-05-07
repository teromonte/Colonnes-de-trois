#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../header/protocolColonne.h"

/*
 * Structure responseAPI
 */
typedef struct
{
  TCoup typeMove;
  TCase placeMove;
  TDeplPion displaceMove;
} responseAI;

// request play
int requestAI(TPion color, int sockAI, responseAI *res);
