#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include "../../header/protocolColonne.h"

/*
 * Structure responseAPI
 */
typedef struct
{
  TCoup typeMove;
  TCase placeMove;
  TDeplPion displaceMove;
} stru2;

// request play
int fun2(TPion color, int sockAI, stru2 *res);
