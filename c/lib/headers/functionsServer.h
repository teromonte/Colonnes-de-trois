#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../../header/protocolColonne.h"

#ifndef FUNCTIONSSERVER_H
#define FUNCTIONSSERVER_H

#define MAX_CLIENT 2

// buildPlayResponse
bool buildPlayResponse(bool verif, int turn, TCoupRep *playRep);

int doHandshake(int sockTrans[]);

#endif