#include "../headers/protocolJava.h"

int requestAI(enum TPion color, int sockAI, struct ResponseAI *res)
{
  int err;
  int moveType;
  int depCol, depLg, arrCol, arrLg;

  err = send(sockAI, &color, sizeof(int), 0);

  err = recv(sockAI, &moveType, sizeof(int), 0);
  err = recv(sockAI, &depCol, sizeof(int), 0);
  err = recv(sockAI, &depLg, sizeof(int), 0);
  err = recv(sockAI, &arrCol, sizeof(int), 0);
  err = recv(sockAI, &arrLg, sizeof(int), 0);

  struct TCase dep = {.col = depCol, .lg = depLg};
  struct TCase arr = {.col = arrCol, .lg = arrLg};
  struct TDeplPion displace = {.caseDep = dep, .caseArr = arr};

  res->typeMove = moveType;
  res->placeMove = dep;
  res->displaceMove = displace;

  return err;
}

void handleOponentPlayInformation(enum TPion color, int sockAI, struct TCoupReq playReq)
{
  int moveType = playReq.typeCoup;
  int depCol = playReq.action.deplPion.caseDep.col;
  int depLg = playReq.action.deplPion.caseDep.lg;
  int arrCol = playReq.action.posPion.col;
  int arrLg = playReq.action.posPion.lg;

  send(sockAI, &color, sizeof(int), 0);
  send(sockAI, &moveType, sizeof(int), 0);
  send(sockAI, &depCol, sizeof(int), 0);
  send(sockAI, &depLg, sizeof(int), 0);
  send(sockAI, &arrCol, sizeof(int), 0);
  send(sockAI, &arrLg, sizeof(int), 0);
}

int setNextStateAI(int sockAI)
{
  int err;
  int i = SET;
  err = send(sockAI, &i, sizeof(int), 0);
  printf("(Client) Asking Server AI to set NEXT STATE!");

  return err;
}

int buildPlayRequest(int playerColor, struct ResponseAI *javaAPIRes, struct TCoupReq *playReq)
{
  playReq->typeCoup = javaAPIRes->typeMove;
  playReq->coul = playerColor; // color
  playReq->idRequest = COUP;
  switch (playReq->typeCoup)
  {
  case POS_PION:
    playReq->action.posPion = javaAPIRes->placeMove;
    printf("(Client) Player %d sent new play of type POS_PION, in  C: %d; L: %d\n",
           playerColor, playReq->action.posPion.lg, playReq->action.posPion.col);
    break;
  case DEPL_PION:
    playReq->action.deplPion = javaAPIRes->displaceMove;
    printf("(Client) Player %d sent new play of type DEPL_PION, C: %d; L: %d => C: %d; L: %d\n",
           playerColor, playReq->action.deplPion.caseDep.lg, playReq->action.deplPion.caseDep.col,
           playReq->action.deplPion.caseArr.lg, playReq->action.deplPion.caseArr.col);
    break;
  case PASSE:
    printf("(Client) Player %d sent new play of type DEPL_PION\n", playerColor);
    break;
  }

  return 1;
}

bool makeMove(int playerColor, int sockAI, int sockC, struct TCoupRep *playRes1)
{
  // Request play to Server IA and send to Server C
  struct ResponseAI javaAPIRes1;
  requestAI(playerColor, sockAI, &javaAPIRes1);
  struct TCoupReq playReq1;
  buildPlayRequest(playerColor, &javaAPIRes1, &playReq1);
  send(sockC, &playReq1, sizeof(struct TCoupReq), 0);

  // Receive & Treat OwnPlayValidation
  int err;
  struct TCoupRep temp;
  err = recv(sockC, &temp, sizeof(struct TCoupRep), 0);
  playRes1->err = temp.err;
  playRes1->propCoup = temp.propCoup;
  playRes1->validCoup = temp.validCoup;

  return err > 0;
}
