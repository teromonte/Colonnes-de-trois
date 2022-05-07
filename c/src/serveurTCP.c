#include "../header/protocolColonne.h"
#include "../lib/headers/fonctionsTCP.h"
#include "../lib/headers/validation.h"

#include <stdbool.h>
#define MAX_CLIENT 2

int main(int argc, char **argv)
{
  int sockConx, /* descripteur socket connexion */
      port,     /* numero de port */
      sizeAddr, /* taille de l'adresse d'une socket */
      err;

  int sockTrans[MAX_CLIENT];
  struct sockaddr_in addClient; /* adresse de la socket client connectee */

  int nPlays0 = 0, nPlays1 = 0;

  char nom1[TNOM], nom2[TNOM];

  TPartieReq matchReq;
  TPartieRep matchRep;
  TCoupReq playReq;
  TCoupRep playRep;
  bool verif;

  // verification des arguments
  if (argc != 2)
  {
    printf("usage : %s port\n", argv[0]);
    return -1;
  }

  port = atoi(argv[1]);
  sockConx = socketServeur(port);
  sizeAddr = sizeof(struct sockaddr_in);

  ////////////// WAIT CONNECTION ////////////
  int k = 0;
  while (k < MAX_CLIENT)
  {
    sockTrans[k] = accept(sockConx, (struct sockaddr *)&addClient, (socklen_t *)&sizeAddr);
    if (sockTrans[k] < 0)
    {
      perror("(serveurTCP) erreur sur accept");
      return -5;
    }

    k++;

    printf("acc = %d \n", k);
  }
  printf("(serveur) Received both connections request\n");
  ////////////// RECEIVE PLAYES REQUEST ////////////
  int i = 0;
  while (i < MAX_CLIENT)
  {
    printf("Receive match request in sockTrans[%d]\n", i);
    err = recv(sockTrans[i], &matchReq, sizeof(TPartieReq), 0);
    switch (matchReq.idRequest)
    {
    case PARTIE:
      if (i == 0)
      {
        strncpy(nom1, matchReq.nomJoueur, TNOM);
        printf("(serveurTCP) Name received: %s\n", nom1);
      }
      else if (i == 1)
      {
        strncpy(nom2, matchReq.nomJoueur, TNOM);
        printf("(serveurTCP) Name received: %s\n", nom2);
      }
      break;
    default:
      printf("(serveurTCP) Error while receiving MATCH REQUEST\n");
      return -1;
      break;
    }
    i++;
  }
  printf("(serveur) Received both matches request\n");
  ////////////// SEND ACK TO PLAYERS ////////////
  int j = 0;
  matchRep.err = ERR_OK;
  while (j < MAX_CLIENT)
  {
    if (j == 0)
    {
      matchRep.coul = BLANC;
      strncpy(matchRep.nomAdvers, nom2, TNOM);
      printf("(serveur) Sending match response to player: %s, with color: %d, to sockTrans[%d]\n", nom1, matchRep.coul, j);
    }
    else if (j == 1)
    {
      matchRep.coul = NOIR;
      strncpy(matchRep.nomAdvers, nom1, TNOM);
      printf("(serveur) Sending match response to player: %s, with color: %d, to sockTrans[%d]\n", nom2, matchRep.coul, j);
    }
    err = send(sockTrans[j], &matchRep, sizeof(TPartieRep), 0);
    j++;
  }
  printf("(serveur) Sent Match response to participants\n");

  printf("\n");
  printf("FINISHED BUROCRACY\n");

  ////////////// GAME START ////////////
  int match = 0;
  bool matchRunning = true;
  int turn = 0;

  while (match < NUM_OF_MATCHES) // server
  {
    initialiserPartie();

    printf("\n");
    printf("(serveur) Starting match number %d!\n", match);
    printf("\n");
    while (nPlays0 < 200 && nPlays1 < 200 && matchRunning)
    {
      // receive coup
      printf("(serveur) Receiving the play request from player %d\n", turn);
      err = recv(sockTrans[turn], &playReq, sizeof(TCoupReq), 0);

      // validate coup
      verif = validationCoup(turn + 1, playReq, &playRep.propCoup);
      printf("(serveur) Just verified pos\n");
      // Create response Crep COUP du J1

      switch (verif)
      {
      case true:
        playRep.err = ERR_OK;
        switch (playRep.propCoup)
        {
        case CONT:
          printf("(serveur) No one won the game! Let's continue!\n");
          playRep.validCoup = VALID;
          break;
        case NULLE:
          printf("(serveur) resultats est egeau\n");
          playRep.validCoup = TIMEOUT;
          matchRunning = false;
          break;
        case GAGNE:
          printf("(serveur) The first player gagne\n");
          playRep.validCoup = VALID;
          matchRunning = false;
          break;
        case PERDU:
          printf("(serveur) The seconde player gagne\n");
          playRep.validCoup = TRICHE;
          matchRunning = false;
          break;
        }
        break;
      case false:
        matchRunning = false;
        playRep.err = ERR_COUP;
        printf("(serveur) Play not valid, ending match...\n");
        break;
      }

      // send response to players
      printf("(serveur) Sending play acknolegment to both players!\n");
      for (int i = 0; i < 2; i++)
      {
        err = send(sockTrans[i], &playRep, sizeof(TCoupRep), 0);
      }

      if (turn == 0)
      {
        nPlays0++;
        turn = 1;
      }
      else
      {
        nPlays1++;
        turn = 0;
      }
    }
    match++;
    nPlays1 = 0;
    nPlays0 = 0;
    turn = 1;
    matchRunning = true;
  }

  printf("THE END\n");

  // Close communication
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    shutdown(sockTrans[i], SHUT_RDWR);
    close(sockTrans[i]);
    close(sockConx);
  }

  return 0;
}
