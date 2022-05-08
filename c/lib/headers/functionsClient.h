#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "../../header/protocolColonne.h"
#include "../../header/responseAI.h"
#include "protocolJava.h"

#ifndef FUNCTIONSCLIENT_H
#define FUNCTIONSCLIENT_H

// Handler
int handleParticipationAck(TPartieRep participationRes);

// Handler
bool handleOwnPlayValidation(TCoupRep playRes);

// Handler
bool handleOponentPlayValidation(TCoupRep playRes);

// buildPlayRequest
int buildPlayRequest(int playerColor, struct ResponseAI *javaAPIRes, TCoupReq *playReq);

// make move
bool makeMove(int playerColor, int sockAI, int sockC);

// Do handshake
int doHandshake(int sockC, char *chaine);

#endif