#include "../header/protocolColonne.h"
#include "../lib/headers/fonctionsTCP.h"
#include "../lib/headers/validation.h"
#include "../lib/headers/functionsServer.h"

#include <stdbool.h>
#define MAX_CLIENT 2

int main(int argc, char **argv)
{
  int sockConx, /* descripteur socket connexion */
      port,     /* numero de port */
      sizeAddr, /* taille de l'adresse d'une socket */
      nPlays0 = 0,
      nPlays1 = 0,
      match = 0,
      sockTrans[MAX_CLIENT];

  struct sockaddr_in addClient; /* adresse de la socket client connectee */

  bool matchRunning = true,
       verif;

  char nom1[TNOM],
      nom2[TNOM];

  TPartieReq matchReq;
  TPartieRep matchRep;
  TCoupReq playReq;
  TCoupRep playRep;

  ////////////// VERIFY INPUT ////////////
  if (argc != 2)
  {
    printf("usage : %s port\n", argv[0]);
    return -1;
  }
  else
  {
    port = atoi(argv[1]);
    sockConx = socketServeur(port);
    sizeAddr = sizeof(struct sockaddr_in);
  }

  ////////////// WAIT CONNECTION ////////////
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    sockTrans[i] = accept(sockConx, (struct sockaddr *)&addClient, (socklen_t *)&sizeAddr);
    if (sockTrans[i] < 0)
    {
      printf("Problem to receive connection!\n");
      return -1;
    }
    printf("(serveurTCP) Player number %d, connection accepted\n", i);
  }

  ////////////// RECEIVE PLAYERS REQUEST /////
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    recv(sockTrans[i], &matchReq, sizeof(TPartieReq), 0);
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
      printf("(serveurTCP) Received COUP request instead of PARTIE, going down\n");
      return -1;
      break;
    }
  }

  ////////////// SEND ACK TO PLAYERS ////////
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    if (i == 0)
    {
      matchRep.coul = BLANC;
      strcpy(matchRep.nomAdvers, nom2);
      printf("(serveur) Sending match response to player: %s, with color: %d, to sockTrans[%d]\n", nom1, matchRep.coul, i);
    }
    else
    {
      matchRep.coul = NOIR;
      strcpy(matchRep.nomAdvers, nom1);
      printf("(serveur) Sending match response to player: %s, with color: %d, to sockTrans[%d]\n", nom2, matchRep.coul, i);
    }
    matchRep.err = ERR_OK;
    send(sockTrans[i], &matchRep, sizeof(TPartieRep), 0);
  }

  ////////////// GAME START ////////////

  while (match < NUM_OF_MATCHES) // server
  {
    printf("\n");
    printf("(serveur) Starting match number %d!\n", match);
    printf("\n");

    initialiserPartie();

    int turn = 0;

    while (nPlays0 < 2000 && nPlays1 < 2000 && matchRunning)
    {
      // receive coup
      printf("(serveur) Receiving the play request from player %d\n", turn);
      recv(sockTrans[turn], &playReq, sizeof(TCoupReq), 0);

      // validate coup
      verif = validationCoup(turn + 1, playReq, &playRep.propCoup);
      switch (verif)
      {
      case true:
        playRep.err = ERR_OK;
        switch (playRep.propCoup)
        {
        case CONT:
          printf("(serveur) Play from player %d resulted in CONT\n", turn);
          playRep.validCoup = VALID;
          break;
        case NULLE:
          printf("(serveur) Play from player %d resulted in NULLE\n", turn);
          playRep.validCoup = TIMEOUT;
          matchRunning = false;
          break;
        case GAGNE:
          printf("(serveur) Play from player %d resulted in GAGNE\n", turn);
          playRep.validCoup = VALID;
          matchRunning = false;
          break;
        case PERDU:
          printf("(serveur) Play from player %d resulted in PERDU\n", turn);
          playRep.validCoup = TRICHE;
          matchRunning = false;
          break;
        }
        break;
      case false:
        playRep.err = ERR_COUP;
        matchRunning = false;
        printf("(serveur) Play from player %d is NOT VALID!\n", turn);
        break;
      }

      // send ack to players
      printf("(serveur) Sending play acknolegment to both players!\n");
      for (int i = 0; i < 2; i++)
      {
        send(sockTrans[i], &playRep, sizeof(TCoupRep), 0);
      }

      if (turn == 0 && matchRunning)
      {
        nPlays0++;
        turn = 1;
      }
      else if (turn == 1 && matchRunning)
      {
        nPlays1++;
        turn = 0;
      }

      printf("\n");
    }

    match++;
    nPlays1 = 0;
    nPlays0 = 0;
    matchRunning = true;

    int temp = sockTrans[0];
    sockTrans[0] = sockTrans[1];
    sockTrans[1] = temp;
  }

  ////////////// CLOSE COMMUNICATION ////////
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    shutdown(sockTrans[i], SHUT_RDWR);
    close(sockTrans[i]);
    close(sockConx);
  }

  printf("GAME FINISHED, TURNING OFF THE SERVER!!!\n");
  return 0;
}