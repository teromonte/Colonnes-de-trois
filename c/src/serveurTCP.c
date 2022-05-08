#include "../lib/headers/functionsServer.h"
#include "../lib/headers/validation.h"
#include "../header/protocolColonne.h"
#include "../lib/headers/fonctionsTCP.h"

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

  ////////////// VERIFY INPUT ////////////
  if (argc != 2)
  {
    printf("usage : %s port\n", argv[0]);
    return -1;
  }
  port = atoi(argv[1]);
  sockConx = socketServeur(port);
  sizeAddr = sizeof(struct sockaddr_in);

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

  ////////////// DO HANDSHAKE ////////////
  doHandshake(sockTrans);

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
      TCoupReq playReq;
      printf("(serveur) Receiving the play request from player %d\n", turn);
      recv(sockTrans[turn], &playReq, sizeof(TCoupReq), 0);

      // validate coup
      TCoupRep playRep;
      verif = validationCoup(turn + 1, playReq, &playRep.propCoup);
      matchRunning = buildPlayResponse(verif, turn, &playRep);

      // send ack to players
      printf("(serveur) Sending play acknolegment to both players!\n");
      for (int i = 0; i < 2; i++)
        send(sockTrans[i], &playRep, sizeof(TCoupRep), 0);

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