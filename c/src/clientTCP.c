#include "../lib/headers/protocolJava.h"
#include "../lib/headers/functionsClient.h"
#include "../header/protocolColonne.h"
#include "../lib/headers/fonctionsTCP.h"

#include <stdbool.h>

int main(int argc, char **argv)
{
  int sockC,
      portC,  /* variables de lecture */
      sockAI, /* descripteur de la socket locale */
      portAI,
      playerColor;

  char *nomMachServ, *playerName;

  ////////////// VERIFY INPUT ////////////
  if (argc != 5)
  {
    printf("usage : %s nom/IPServ portC portAI playerName.\n", argv[0]);
    return -1;
  }
  nomMachServ = argv[1];
  portC = atoi(argv[2]);
  portAI = atoi(argv[3]);
  playerName = argv[4];

  ////////////// CONNECT TO C SERVER ////////////
  sockC = socketClient(nomMachServ, portC);
  printf("(Client) Connected to C Server!\n");

  ////////////// DO HANDSHAKE ////////////
  playerColor = doHandshake(sockC, playerName);

  ////////////// CONNECT TO JAVA SERVER AND START IT ////////////
  sockAI = socketClient(nomMachServ, portAI);
  printf("(Client) Connected to JAVA Server!\n");
  startAI(sockAI, playerColor);

  /////////// PLAYS START ////////////////
  int matchNumber = 0;
  while (matchNumber < NUM_OF_MATCHES)
  {
    bool matchIsOn = true;

    printf("\n");
    printf("(Client) Match number %d is about to START!\n", matchNumber);

    // Do first play of the match
    if (((playerColor == BLANC && matchNumber == 0) ||
         (playerColor == NOIR && matchNumber == 1)))
    {
      printf("\n");
      struct TCoupRep plasyRes;
      makeMove(playerColor, sockAI, sockC, &plasyRes);
      matchIsOn = handleOwnPlayValidation(plasyRes);
    }

    // Start to send plays
    while (matchIsOn)
    {
      // Receive & Treat Oponent Play Validation
      struct TCoupRep playRes1;
      recv(sockC, &playRes1, sizeof(struct TCoupRep), 0);
      matchIsOn = handleOponentPlayValidation(playRes1);

      // Make new play
      if (matchIsOn)
      {
        // Receive & Treat Oponent Play information
        struct TCoupReq playReq;
        recv(sockC, &playReq, sizeof(struct TCoupReq), 0);
        handleOponentPlayInformation(playReq.coul, sockAI, playReq);

        struct TCoupRep playRes2;
        makeMove(playerColor, sockAI, sockC, &playRes2);
        matchIsOn = handleOwnPlayValidation(playRes2);
      }
    }

    // Prepare for next match
    setNextStateAI(sockAI);
    playerColor = 1 - playerColor;
    matchNumber++;
  }

  printf("(Client) GAME FINISHED, TURNING OFF THE SERVER!!!\n");
  close(sockC);
  return 0;
}