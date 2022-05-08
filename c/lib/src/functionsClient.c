#include "../headers/functionsClient.h"

int handleParticipationAck(TPartieRep participationRes)
{
  int playerColor;
  switch (participationRes.err)
  {
  case ERR_OK:
    if (participationRes.coul == BLANC)
      printf("(Client) Your color is BLANC and you are playing against: %s\n", participationRes.nomAdvers);
    else
      printf("(Client) Your color is NOIR and you are playing against: %s\n", participationRes.nomAdvers);
    playerColor = participationRes.coul;
    break;
  case ERR_PARTIE:
    printf("(Client) Error PARTIE, cannot participate!\n");
    exit(0);
  case ERR_TYP:
    printf("(Client) Error TYP, cannot participate!\n");
    exit(0);
  case ERR_COUP:
    printf("(Client) Error COUP, cannot participate!\n");
    exit(0);
  }
  return playerColor;
}

bool handleOwnPlayValidation(TCoupRep playRes)
{
  bool matchIsOn = true;
  switch (playRes.err)
  {
  case ERR_OK:
    switch (playRes.validCoup)
    {
    case VALID:
      switch (playRes.propCoup)
      {
      case CONT:
        printf("(Client) My play is VALID and resulted in CONT!\n");
        break;
      case GAGNE:
        printf("(Client) My play is VALID and resulted in GAGNE!\n");
        matchIsOn = false;
        break;
      case NULLE:
        printf("(Client) My play is VALID and resulted in NULLE!\n");
        matchIsOn = false;
        break;
      case PERDU:
        printf("(Client) My play is VALID and resulted in PERDU!\n");
        matchIsOn = false;
        break;
      }
      break;
    case TIMEOUT:
      printf("(Client) My play is no VALID and resulted in TIMEOUT!\n");
      matchIsOn = false;
      break;
    case TRICHE:
      printf("(Client) My play is no VALID and resulted in TRICHE!\n");
      matchIsOn = false;
      break;
    }
    break;
  case ERR_COUP:
    printf("(Client) My play is NOT VALID and resulted in ERR_COUP!\n");
    matchIsOn = false;
    break;
  case ERR_TYP:
    printf("(Client) My play is NOT VALID and resulted in ERR_TYP!\n");
    matchIsOn = false;
    break;
  case ERR_PARTIE:
    printf("(Client) My play is NOT VALID and resulted in ERR_PARTIE!\n");
    matchIsOn = false;
    break;
  }
  printf("\n");
  return matchIsOn;
}

bool handleOponentPlayValidation(TCoupRep playRes)
{
  bool matchIsOn = true;
  switch (playRes.err)
  {
  case ERR_OK:
    switch (playRes.validCoup)
    {
    case VALID:
      switch (playRes.propCoup)
      {
      case CONT:
        printf("(Client) Adversaire play was VALID and resulted in CONT\n");
        break;
      case GAGNE:
        printf("(Client) Adversaire play was VALID and resulted in GAGNE\n");
        matchIsOn = false;
        break;
      case NULLE:
        printf("(Client) Adversaire play was VALID and resulted in NULLE\n");
        matchIsOn = false;
        break;
      case PERDU:
        printf("(Client) Adversaire play was VALID and resulted in PERDU\n");
        matchIsOn = false;
        break;
      }
      break;
    case TIMEOUT:
      printf("(Client) Adversaire play was NOT VALID and resulted in TIMEOUT\n");
      matchIsOn = false;
      break;
    case TRICHE:
      printf("(Client) Adversaire play was NOT VALID and resulted in TRICHE\n");

      matchIsOn = false;
      break;
    }
    break;
  case ERR_COUP:
    printf("(Client) Adversaire play was NOT VALID and resulted in ERR_COUP\n");
    matchIsOn = false;
    break;
  case ERR_TYP:
    printf("(Client) Adversaire play was NOT VALID and resulted in ERR_TYP\n");
    matchIsOn = false;
    break;
  case ERR_PARTIE:
    printf("(Client) Adversaire play was NOT VALID and resulted in ERR_PARTIE\n");
    matchIsOn = false;
    break;
  }
  return matchIsOn;
}

int buildPlayRequest(int playerColor, struct ResponseAI *javaAPIRes, TCoupReq *playReq)
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

bool makeMove(int playerColor, int sockAI, int sockC)
{
  // Request play to Server IA and send to Server C
  struct ResponseAI javaAPIRes1;
  requestAI(playerColor, sockAI, &javaAPIRes1);
  TCoupReq playReq1;
  buildPlayRequest(playerColor, &javaAPIRes1, &playReq1);
  send(sockC, &playReq1, sizeof(TCoupReq), 0);

  // Receive & Treat OwnPlayValidation
  TCoupRep playRes1;
  recv(sockC, &playRes1, sizeof(TCoupRep), 0);
  
  return handleOwnPlayValidation(playRes1);
}