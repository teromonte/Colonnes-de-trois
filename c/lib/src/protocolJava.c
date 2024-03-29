#include "../headers/protocolJava.h"

int requestAI(enum TPion color, int sockAI, struct ResponseAI *res)
{

  printf("(Client) Sent new play request to AI Server!\n");

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

int sendOpponentMoveToAI(enum TPion color, int sockAI, struct TCoupReq playReq)
{
  int err;
  int moveType = playReq.typeCoup;
  int depCol = playReq.action.posPion.col;
  int depLg = playReq.action.posPion.lg;
  int arrCol = playReq.action.deplPion.caseArr.col;
  int arrLg = playReq.action.deplPion.caseArr.lg;

  err = send(sockAI, &color, sizeof(int), 0);
  err = send(sockAI, &moveType, sizeof(int), 0);
  err = send(sockAI, &depCol, sizeof(int), 0);
  err = send(sockAI, &depLg, sizeof(int), 0);
  err = send(sockAI, &arrCol, sizeof(int), 0);
  err = send(sockAI, &arrLg, sizeof(int), 0);
  return err;
}

void handleOponentPlayInformation(enum TPion color, int sockAI, struct TCoupReq playReq)
{
  int err;
  err = sendOpponentMoveToAI(color, sockAI, playReq);
  if (err > 0)
  {
    switch (playReq.typeCoup)
    {
    case POS_PION:
      printf("(Client) Opponent play sent to AI! POS_PION Case (%d,%d).\n",
             playReq.action.posPion.col, playReq.action.posPion.lg);
      break;
    case DEPL_PION:
      printf("(Client) Opponent play sent to AI! DEPL_PION Case (%d,%d) -> Case (%d,%d).\n",
             playReq.action.deplPion.caseDep.col, playReq.action.deplPion.caseDep.lg,
             playReq.action.deplPion.caseArr.col, playReq.action.deplPion.caseArr.lg);
      break;
    case PASSE:
      printf("(Client) Opponent play sent to AI! PASSE!\n");
      break;
    }
  }
  else
  {
    printf("(Client) Fail to send Opponent play to AI!");
    exit(0);
  }
}

int startAI(int sockAI, int playerColor)
{
  int err;
  err = send(sockAI, &playerColor, sizeof(int), 0);
  printf("(Client) Asking Server AI to START!");

  return err;
}

int setNextStateAI(int sockAI)
{
  int err;
  int i = SET;
  err = send(sockAI, &i, sizeof(int), 0);
  printf("(Client) Asking Server AI to set NEXT STATE!\n");
  printf("\n");
  return err;
}

void buildPlayRequest(int playerColor, struct ResponseAI *javaAPIRes, struct TCoupReq *playReq)
{
  playReq->typeCoup = javaAPIRes->typeMove;
  playReq->coul = playerColor; // color
  playReq->idRequest = COUP;
  switch (playReq->typeCoup)
  {
  case POS_PION:
    playReq->action.posPion = javaAPIRes->placeMove;
    printf("(Client) Opponent play sent to AI! POS_PION Case (%d,%d).\n",
           playReq->action.posPion.col, playReq->action.posPion.lg);
    break;
  case DEPL_PION:
    playReq->action.deplPion = javaAPIRes->displaceMove;
    printf("(Client) Opponent play sent to AI! DEPL_PION Case (%d,%d) -> Case (%d,%d).\n",
           playReq->action.deplPion.caseDep.col, playReq->action.deplPion.caseDep.lg,
           playReq->action.deplPion.caseArr.col, playReq->action.deplPion.caseArr.lg);
    break;
  case PASSE:
    printf("(Client) Player %d sent new play of type PASSE!\n", playerColor);
    break;
  }
}

void makeMove(int playerColor, int sockAI, int sockC, struct TCoupRep *playRes1)
{
  // Request play to Server IA and send to Server C
  struct ResponseAI javaAPIRes1;
  requestAI(playerColor, sockAI, &javaAPIRes1);
  struct TCoupReq playReq1;
  buildPlayRequest(playerColor, &javaAPIRes1, &playReq1);
  send(sockC, &playReq1, sizeof(struct TCoupReq), 0);

  // Receive & Treat OwnPlayValidation
  struct TCoupRep temp;
  recv(sockC, &temp, sizeof(struct TCoupRep), 0);
  playRes1->err = temp.err;
  playRes1->propCoup = temp.propCoup;
  playRes1->validCoup = temp.validCoup;
}
