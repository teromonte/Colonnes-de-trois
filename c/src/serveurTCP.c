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

  /*
   * verification des arguments
   */
  if (argc != 2)
  {
    printf("usage : %s port\n", argv[0]);
    return -1;
  }

  port = atoi(argv[1]);
  sockConx = socketServeur(port);
  sizeAddr = sizeof(struct sockaddr_in);

  // attente de connexion
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
  // Receive match request of participate
  int i = 0;
  while (i < MAX_CLIENT)
  {
    printf("Receive match request in sockTrans[%d]\n", i);
    err = recv(sockTrans[i], &matchReq, sizeof(TPartieReq), 0);
    if (err < 0)
    {
      perror("(serveur) Error recv Match request");
      return -10;
    }
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
  // Match response to the participants
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
    if (err < 0)
    {
      perror("(serveur) error sending match responde to player " + i);
      return -11;
    }
    j++;
  }
  printf("(serveur) Sent Match response to participants\n");

  // Start Game
  int match = 0;
  int turn = 0;
  bool matchRunning = true;
  printf("(serveur) initialiserPartie\n");
  initialiserPartie();
  while (match < 2) // server
  {
    while (nPlays0 < 20 && nPlays1 < 20 && matchRunning)
    {
      // receive coup
      printf("(serveur) Receiving the play request from player %d\n", turn);
      err = recv(sockTrans[turn], &playReq, sizeof(TCoupReq), 0);
      if (err < 0)
      {
        perror("(serveur) Error receiving the play request from player");
        return -10;
      }

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
        case NULLE:
          printf("(serveur) resultats est egeau\n");
          playRep.validCoup = TIMEOUT;
          break;

        case GAGNE:
          printf("(serveur) the first player gagne\n");
          playRep.validCoup = VALID;
          match++;
          break;

        case PERDU:
          printf("(serveur) the seconde player gagne\n");
          playRep.validCoup = TRICHE;
          match++;
          break;

        case CONT:
          printf("(serveur) no one won the game!!\n");
          playRep.validCoup = VALID;
          break;
        }
        break;
      case false:
        matchRunning = false;
        printf("(serveur) Play not valid, ending match...\n");
        break;
      }

      // send response to players
      for (int i = 0; i < 2; i++)
      {
        printf("(serveur) Sending play acknolegment to player %d\n", i);
        err = send(sockTrans[i], &playRep, sizeof(TCoupRep), 0);
        if (err < 0)
        {
          perror("(serveur) Error sending play acknolegment to player");
          return -11;
        }
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
    matchRunning = true;

    printf("(serveur) Going to second match\n");
    turn = 1;
  }
  printf("THE END");
  printf("\n");

  /*
   * arret de la connexion et fermeture
   */
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    shutdown(sockTrans[i], SHUT_RDWR);
    close(sockTrans[i]);
    close(sockConx);
  }

  return 0;
}
