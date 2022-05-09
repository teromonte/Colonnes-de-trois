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
      sockTrans[MAX_CLIENT];

  struct sockaddr_in addClient; /* adresse de la socket client connectee */

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
  int match = 0;
  while (match < NUM_OF_MATCHES) // server
  {
    printf("\n");
    printf("(serveur) Starting match number %d!\n", match);
    printf("\n");

    initialiserPartie();

    int turn = 0;
    bool matchRunning = true;
    while (matchRunning)
    {
      // receive coup
      struct TCoupReq playReq;
      printf("\n");
      printf("(serveur) Receiving the play request from player %d\n", turn);
      printf("\n");
      recv(sockTrans[turn], &playReq, sizeof(struct TCoupReq), 0);

      // validate coup
      struct TCoupRep playRep;
      bool verif = validationCoup(turn + 1, playReq, &playRep.propCoup);
      matchRunning = validateAndBuildPlayResponse(verif, turn, &playRep);

      // send ack to players
      printf("(serveur) Sending play acknolegment to both players!\n");
      for (int i = 0; i < MAX_CLIENT; i++)
        send(sockTrans[i], &playRep, sizeof(struct TCoupRep), 0);

      // Send what was played to the opponent
      if (matchRunning)
      {
        send(sockTrans[1 - turn], &playReq, sizeof(struct TCoupReq), 0);
        printf("(serveur) Sending play request to opponent player %d!\n", 1 - turn);
      }

      // change turns and increment play
      turn = 1 - turn;
    }

    printf("\n");
    match++;
  }

  ////////////// CLOSE COMMUNICATION ////////
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    shutdown(sockTrans[i], SHUT_RDWR);
    close(sockTrans[i]);
    close(sockConx);
  }

  printf("(Server) GAME FINISHED, TURNING OFF THE SERVER!!!\n");
  return 0;
}