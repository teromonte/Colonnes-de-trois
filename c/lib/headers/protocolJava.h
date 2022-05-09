#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../header/protocolColonne.h"
#include "../../header/responseAI.h"

#ifndef PROTOCOLJAVA_H
#define PROTOCOLJAVA_H

// request play
int requestAI(enum TPion color, int sockAI, struct ResponseAI *res);

// initialise AI
int setNextStateAI(int sockAI);

// buildPlayRequest
int buildPlayRequest(int playerColor, struct ResponseAI *javaAPIRes, struct TCoupReq *playReq);

// make move
bool makeMove(int playerColor, int sockAI, int sockC, struct TCoupRep *playRes1);

// send info coup advers
void handleOponentPlayInformation(enum TPion color, int sockAI, struct TCoupReq playReq);

#endif