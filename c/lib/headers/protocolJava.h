#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../header/protocolColonne.h"
#include "../../header/responseAI.h"

#ifndef PROTOCOLJAVA_H
#define PROTOCOLJAVA_H

// request play
int requestAI(TPion color, int sockAI, struct ResponseAI *res);

#endif