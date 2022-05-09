#include "../headers/functionsClient.h"

int handleParticipationAck(struct TPartieRep participationRes)
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

bool handleOwnPlayValidation(struct TCoupRep playRes)
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

bool handleOponentPlayValidation(struct TCoupRep playRes)
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

int doHandshake(int sockC, char chaine[])
{
  ///////// ASK FOR PARTICIPATION //////////
  struct TPartieReq participationReq;
  strcpy(participationReq.nomJoueur, chaine);
  participationReq.idRequest = PARTIE;
  send(sockC, &participationReq, sizeof(struct TPartieReq), 0);
  printf("(Client) Participation request sent in name of %s!\n", participationReq.nomJoueur);

  ///////// RECIEIVE PARTICIPATION ACK //////////
  struct TPartieRep participationRes;
  recv(sockC, &participationRes, sizeof(struct TPartieRep), 0);
  return handleParticipationAck(participationRes);
}