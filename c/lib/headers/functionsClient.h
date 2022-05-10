#include "../../header/protocolColonne.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#ifndef FUNCTIONSCLIENT_H
#define FUNCTIONSCLIENT_H

// Handler
int handleParticipationAck(struct TPartieRep participationRes);

// Handler
bool handleOwnPlayValidation(struct TCoupRep playRes);

// Handler
bool handleOponentPlayValidation(struct TCoupRep playRes);

// Do handshake
int doHandshake(int sockC, char *chaine);

#endif