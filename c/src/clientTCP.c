#include "../lib/headers/protocolJava.h"
#include "../header/protocolColonne.h"
#include "../lib/headers/fonctionsTCP.h"
#include <stdbool.h>

int main(int argc, char **argv)
{

  int sockC,
      portC,  /* variables de lecture */
      sockAI, /* descripteur de la socket locale */
      portAI,
      err,
      playerColor,
      matchNumber = 0;

  bool matchIsOn = true;

  char chaine[TNOM],
      *nomMachServ;

  TPartieReq participationReq; // Participation request
  TPartieRep participationRep; // Answer to the participation request
  TCoupReq playReq;            // Play request
  TCoupRep playRes;            // Response to the play request
  responseAI javaAPIRes;

  ////////////// VERIFY INPUT ////////////
  if (argc != 4)
  {
    printf("usage : %s nom/IPServ portC portAI\n", argv[0]);
    return -1;
  }
  else
  {
    nomMachServ = argv[1];
    portC = atoi(argv[2]);
    portAI = atoi(argv[3]);
  }

  ////////////// CONNECT TO C SERVER ////////////
  printf("(Client) Connected to C Server!\n");
  sockC = socketClient(nomMachServ, portC);

  /////////////// GET NAME ///////////
  printf("(Client) To play type the player name!\n");
  scanf("%s", chaine);

  ///////// ASK FOR PARTICIPATION //////////
  strcpy(participationReq.nomJoueur, chaine);
  participationReq.idRequest = PARTIE;
  printf("(Client) Participation request sent in name of %s!\n", participationReq.nomJoueur);
  err = send(sockC, &participationReq, sizeof(TPartieReq), 0);

  ///////// RECEIVE PARTICIPATION RESPONSE//////////
  err = recv(sockC, &participationRep, sizeof(TPartieRep), 0);
  switch (participationRep.err)
  {
  case ERR_OK:
    if (participationRep.coul == BLANC)
      printf("(Client) Your color is BLANC and you are playing against: %s\n", participationRep.nomAdvers);
    else
      printf("(Client) Your color is NOIR and you are playing against: %s\n", participationRep.nomAdvers);
    playerColor = participationRep.coul;
    break;
  case ERR_PARTIE:
    printf("(Client) Error PARTIE, cannot participate!\n");
    return -1;
    break;
  case ERR_TYP:
    printf("(Client) Error TYP, cannot participate!\n");
    return -1;
    break;
  case ERR_COUP:
    printf("(Client) Error COUP, cannot participate!\n");
    return -1;
    break;
  }

  /////////// PLAYS START ////////////////
  while (matchNumber < NUM_OF_MATCHES)
  {
    printf("\n");
    printf("(Client) Match number %d is about to START!\n", matchNumber);
    printf("\n");

    // Do first play of the match
    if (playerColor == BLANC && matchIsOn)
    {
      sockAI = socketClient(nomMachServ, portAI);
      err = requestAI(playerColor, sockAI, &javaAPIRes);

      playReq.coul = playerColor; // color
      playReq.idRequest = COUP;
      playReq.typeCoup = javaAPIRes.typeMove;
      playReq.action.posPion = javaAPIRes.placeMove;

      printf("(Client) Player %d sent first play of type POS_PION, in L: %d; C: %d\n",
             playerColor, playReq.action.posPion.lg, playReq.action.posPion.col);

      // Send first play
      err = send(sockC, &playReq, sizeof(TCoupReq), 0);

      // Receive my play validation
      err = recv(sockC, &playRes, sizeof(TCoupRep), 0);
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
    }
    // Start to send plays
    while (matchIsOn)
    {
      // Receive validation of adversaire play
      err = recv(sockC, &playRes, sizeof(TCoupRep), 0);
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

      // Make new play
      if (matchIsOn)
      {
        sockAI = socketClient(nomMachServ, portAI);
        err = requestAI(playerColor, sockAI, &javaAPIRes);

        playReq.typeCoup = javaAPIRes.typeMove;
        playReq.coul = playerColor; // color
        playReq.idRequest = COUP;   // here is always COUP (PLAY)
        switch (playReq.typeCoup)
        {
        case POS_PION:
          playReq.action.posPion = javaAPIRes.placeMove;
          printf("(Client) Player %d sent new play of type POS_PION, in L: %d; C: %d\n",
                 playerColor, playReq.action.posPion.lg, playReq.action.posPion.col);
          break;
        case DEPL_PION:
          playReq.action.deplPion = javaAPIRes.displaceMove;
          printf("(Client) Player %d sent new play of type DEPL_PION, L: %d; C: %d => L: %d; C: %d\n",
                 playerColor, playReq.action.deplPion.caseDep.lg, playReq.action.deplPion.caseDep.col,
                 playReq.action.deplPion.caseArr.lg, playReq.action.deplPion.caseArr.col);
          break;
        case PASSE:
          printf("(Client) Player %d sent new play of type DEPL_PION\n", playerColor);
          break;
        }

        err = send(sockC, &playReq, sizeof(TCoupReq), 0);

        err = recv(sockC, &playRes, sizeof(TCoupRep), 0);
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
      }
      printf("\n");
    }
    // Prepare for next match

    if (matchNumber == 0)
    {
      if (playerColor == BLANC)
        playerColor = NOIR;
      else
        playerColor = BLANC;
    }

    matchIsOn = true;
    matchNumber++;
  }
  printf("GAME FINISHED, TURNING OFF THE SERVER\n");
  close(sockC);
  return 0;
}