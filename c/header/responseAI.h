#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "protocolColonne.h"

#ifndef RESPONSEAI_H
#define RESPONSEAI_H

#define SET 2

/*
 * Structure responseAPI
 */
struct ResponseAI
{
  enum TCoup typeMove;
  struct TCase placeMove;
  struct TDeplPion displaceMove;
};

/*
 * Structure responseAPI
 */
struct SendOpenentPlay
{
  enum TCoup typeMove;
  struct TCase placeMove;
  struct TDeplPion displaceMove;
};

#endif