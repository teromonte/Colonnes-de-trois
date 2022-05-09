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

// validate And Build Play Response
bool validateAndBuildPlayResponse(bool verif, int turn, struct TCoupRep *playRep);

// Do Handshake
int doHandshake(int sockTrans[]);

#endif