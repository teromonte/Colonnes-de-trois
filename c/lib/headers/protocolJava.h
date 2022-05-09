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

// Go to next match AI
int setNextStateAI(int sockAI);

// Start request to AI
int startAI(int sockAI, int playerColor);

// buildPlayRequest
int buildPlayRequest(int playerColor, struct ResponseAI *javaAPIRes, struct TCoupReq *playReq);

// make move
bool makeMove(int playerColor, int sockAI, int sockC, struct TCoupRep *playRes1);

// handleOponentPlayInformation
void handleOponentPlayInformation(enum TPion color, int sockAI, struct TCoupReq playReq);

// send opponent move to ai
int sendOpponentMoveToAI(enum TPion color, int sockAI, struct TCoupReq playReq);

#endif