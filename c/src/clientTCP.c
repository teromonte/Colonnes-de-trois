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
      matchNumber;

  bool matchIsOn;

  char chaine[TNOM], dem, *nomMachServ;

  TPartieReq participationReq; // Participation request
  TPartieRep participationRep; // Answer to the participation request
  TCoupReq playReq;            // Play request
  TCoupRep playRes;            // Response to the play request
  responseAI javaAPIRes;

  // verification des arguments
  if (argc != 4)
  {
    printf("usage : %s nom/IPServ portC portAI\n", argv[0]);
    return -1;
  }

  nomMachServ = argv[1];
  portC = atoi(argv[2]);
  portAI = atoi(argv[3]);

  ////////////// CONNECT TO C SERVER ////////////

  sockC = socketClient(nomMachServ, portC);

  /////////////// GET NAME ///////////

  printf("(client) To play use: p playername\n");
  scanf("%c %s", &dem, chaine);
  switch (dem)
  {
  case 'p':
    printf("(client) My name is: %s.\n", chaine);
    break;
  default:
    printf("(client) Wrong command %c\n", dem);
    return -1;
    break;
  }

  ///////// ASK FOR PARTICIPATION //////////

  strncpy(participationReq.nomJoueur, chaine, TNOM);
  participationReq.idRequest = PARTIE;

  // send participation request
  err = send(sockC, &participationReq, sizeof(TPartieReq), 0);

  // Receive participation request response
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
    printf("(client) Error PARTIE, cannot participate!\n");
    return -1;
    break;
  case ERR_TYP:
    printf("(client) Error TYP, cannot participate!\n");
    return -1;
    break;
  case ERR_COUP:
    printf("(client) Error COUP, cannot participate!\n");
    return -1;
    break;
  }

  printf("\n");
  printf("FINISHED BUROCRACY\n");
  /////////// PLAYS START ////////////////

  matchNumber = 0;
  matchIsOn = true;
  while (matchNumber < NUM_OF_MATCHES)
  {
    printf("\n");
    printf("(Client) Match number %d is about to START!\n", matchNumber);
    printf("\n");

    // Do first play of the match
    if (playerColor == BLANC)
    {
      printf("(Client) You are the first player! Lancer un coup!\n");

      sockAI = socketClient(nomMachServ, portAI);
      err = requestAI(playerColor, sockAI, &javaAPIRes);
      printf("Received from API: type: %d, placeMove col: %d, placeMove lg: %d, displaceMove col: %d, displaceMove lg: %d.\n",
             javaAPIRes.typeMove,
             javaAPIRes.placeMove.col,
             javaAPIRes.placeMove.lg,
             javaAPIRes.displaceMove.caseArr.col,
             javaAPIRes.displaceMove.caseArr.lg);

      // Arrange Play protocol package
      playReq.coul = playerColor; // color
      playReq.idRequest = COUP;   // here is always COUP (PLAY)
      playReq.typeCoup = javaAPIRes.typeMove;
      playReq.action.posPion = javaAPIRes.placeMove;

      // Send first play
      err = send(sockC, &playReq, sizeof(TCoupReq), 0);

      printf("(client) FIRST play of the match has been sent!\n");

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
            printf("(Client) Game continues!\n");
            break;
          case GAGNE:
            printf("(Client)I am winning!\n");
            matchIsOn = false;
            break;
          case NULLE:
            printf("(Client) Ive made a mistake!\n");
            matchIsOn = false;
            break;
          case PERDU:
            printf("(Client) Ive LOST!\n");
            matchIsOn = false;
            break;
          }
          break;
        case TIMEOUT:
          printf("(Client) My sent response got timeout!\n");
          matchIsOn = false;
          break;
        case TRICHE:
          printf("(Client) Iam cheating on the game!\n");
          matchIsOn = false;
          break;
        }
        break;
      case ERR_COUP:
        printf("(Client) My play had an error\n");
        matchIsOn = false;
        break;
      case ERR_TYP:
        printf("(Client) My request type was wrong\n");
        matchIsOn = false;
        break;
      case ERR_PARTIE:
        printf("(Client) My request type was wrong\n");
        matchIsOn = false;
        break;
      }
    }

    // Start to send plays, runs till match is gange, perdu, triche, timeout
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
            printf("(Client) Adversaire played: Game continues!\n");
            break;
          case GAGNE:
            printf("(Client) Adversaire played: I am winning!\n");
            matchIsOn = false;
            break;
          case NULLE:
            printf("(Client) Adversaire played: Ive made a mistake!\n");
            matchIsOn = false;
            break;
          case PERDU:
            printf("(Client) Adversaire played: Ive LOST!\n");
            matchIsOn = false;
            break;
          }
          break;
        case TIMEOUT:
          printf("(Client) Adversaire played: My sent response got timeout!\n");
          matchIsOn = false;
          break;
        case TRICHE:
          printf("(Client) Adversaire played: Iam cheating on the game!\n");
          matchIsOn = false;
          break;
        }
        break;
      case ERR_COUP:
        printf("(Client) Adversaire played: My play had an error\n");
        matchIsOn = false;
        break;
      case ERR_TYP:
        printf("(Client) Adversaire played: My request type was wrong\n");
        matchIsOn = false;
        break;
      case ERR_PARTIE:
        printf("(Client) Adversaire played: My request type was wrong\n");
        matchIsOn = false;
        break;
      }

      if (matchIsOn)
      {
        sockAI = socketClient(nomMachServ, portAI);
        err = requestAI(playerColor, sockAI, &javaAPIRes);
        printf("Received from API: type: %d, placeMove col: %d, placeMove lg: %d, displaceMove col: %d, displaceMove lg: %d.\n",
               javaAPIRes.typeMove,
               javaAPIRes.placeMove.col,
               javaAPIRes.placeMove.lg,
               javaAPIRes.displaceMove.caseArr.col,
               javaAPIRes.displaceMove.caseArr.lg);

        playReq.coul = playerColor; // color
        playReq.idRequest = COUP;   // here is always COUP (PLAY)
        switch (javaAPIRes.typeMove)
        {
        case POS_PION:
          playReq.typeCoup = POS_PION;
          playReq.action.posPion = javaAPIRes.placeMove;
          break;
        case DEPL_PION:
          playReq.typeCoup = DEPL_PION;
          playReq.action.deplPion = javaAPIRes.displaceMove;
          break;
        case PASSE:
          playReq.typeCoup = PASSE;
          break;
        }

        // Send play
        err = send(sockC, &playReq, sizeof(TCoupReq), 0);

        if (playerColor == BLANC)
          printf("(client) BLANC send play!\n");
        else
          printf("(client) NOIR send play!\n");

        // Receive validation of own play
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
              printf("(Client) Game continues!\n");
              break;
            case GAGNE:
              printf("(Client)I am winning!\n");
              matchIsOn = false;
              break;
            case NULLE:
              printf("(Client) Ive made a mistake!\n");
              matchIsOn = false;
              break;
            case PERDU:
              printf("(Client) Ive LOST!\n");
              matchIsOn = false;
              break;
            }
            break;
          case TIMEOUT:
            printf("(Client) My sent response got timeout!\n");
            matchIsOn = false;
            break;
          case TRICHE:
            printf("(Client) Iam cheating on the game!\n");
            matchIsOn = false;
            break;
          }
          break;
        case ERR_COUP:
          printf("(Client) My play had an error\n");
          matchIsOn = false;
          break;
        case ERR_TYP:
          printf("(Client) My request type was wrong\n");
          matchIsOn = false;
          break;
        case ERR_PARTIE:
          printf("(Client) My request type was wrong\n");
          matchIsOn = false;
          break;
        }
      }
    }

    // Prepare for next match
    if (matchNumber == 0)
    {
      printf("(client) Players changed colors!\n");
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