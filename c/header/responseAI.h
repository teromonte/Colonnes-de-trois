#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "protocolColonne.h"

#ifndef RESPONSEAI_H
#define RESPONSEAI_H

/*
 * Structure responseAPI
 */
struct ResponseAI
{
  TCoup typeMove;
  TCase placeMove;
  TDeplPion displaceMove;
};

#endif