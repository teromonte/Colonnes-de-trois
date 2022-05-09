#include "../headers/functionsServer.h"

bool validateAndBuildPlayResponse(bool verif, int turn, struct TCoupRep *playRep)
{
  bool matchRunning = true;
  switch (verif)
  {
  case true:
    playRep->err = ERR_OK;
    switch (playRep->propCoup)
    {
    case CONT:
      printf("(serveur) Play from player %d resulted in CONT!\n", turn);
      playRep->validCoup = VALID;
      break;
    case NULLE:
      printf("(serveur) Play from player %d resulted in NULLE!\n", turn);
      playRep->validCoup = TIMEOUT;
      matchRunning = false;
      break;
    case GAGNE:
      printf("(serveur) Play from player %d resulted in GAGNE!\n", turn);
      playRep->validCoup = VALID;
      matchRunning = false;
      break;
    case PERDU:
      printf("(serveur) Play from player %d resulted in PERDU!\n", turn);
      playRep->validCoup = TRICHE;
      matchRunning = false;
      break;
    }
    break;
  case false:
    playRep->err = ERR_COUP;
    matchRunning = false;
    printf("(serveur) Play from player %d is NOT VALID!\n", turn);
    break;
  }
  return matchRunning;
}

int doHandshake(int sockTrans[])
{
  ////////////// RECEIVE PLAYERS REQUEST /////
  char nom1[TNOM], nom2[TNOM];

  for (int i = 0; i < MAX_CLIENT; i++)
  {
    struct TPartieReq matchReq;
    recv(sockTrans[i], &matchReq, sizeof(struct TPartieReq), 0);
    switch (matchReq.idRequest)
    {
    case PARTIE:
      if (i == 0)
      {
        strcpy(nom1, matchReq.nomJoueur);
        printf("(serveurTCP) Name received from player %d: %s.\n", i, nom1);
      }
      else
      {
        strcpy(nom2, matchReq.nomJoueur);
        printf("(serveurTCP) Name received from player %d: %s.\n", i, nom2);
      }
      break;
    case COUP:
      printf("(serveurTCP) Received COUP request instead of PARTIE, going down!\n");
      exit(0);
      break;
    }
  }

  ////////////// SEND ACK TO PLAYERS ////////
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    struct TPartieRep matchRep;
    matchRep.err = ERR_OK;
    if (i == 0)
    {
      matchRep.coul = BLANC;
      strcpy(matchRep.nomAdvers, nom2);
      printf("(serveur) Sending match response to player: %s, with color: %d, to sockTrans[%d]!\n",
             nom1, matchRep.coul, i);
    }
    else
    {
      matchRep.coul = NOIR;
      strcpy(matchRep.nomAdvers, nom1);
      printf("(serveur) Sending match response to player: %s, with color: %d, to sockTrans[%d]!\n",
             nom2, matchRep.coul, i);
    }
    send(sockTrans[i], &matchRep, sizeof(struct TPartieRep), 0);
  }
  return 1;
}