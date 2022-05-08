#include "../lib/headers/protocolJava.h"
#include "../lib/headers/functionsClient.h"
#include "../header/protocolColonne.h"
#include "../header/responseAI.h"
#include "../lib/headers/fonctionsTCP.h"
#include <stdbool.h>

int main(int argc, char **argv)
{
  int sockC,
      portC,  /* variables de lecture */
      sockAI, /* descripteur de la socket locale */
      portAI,
      playerColor,
      matchNumber = 0;

  bool matchIsOn = true;

  char chaine[TNOM],
      *nomMachServ;

  ////////////// VERIFY INPUT ////////////
  if (argc != 4)
  {
    printf("usage : %s nom/IPServ portC portAI.\n", argv[0]);
    return -1;
  }
  nomMachServ = argv[1];
  portC = atoi(argv[2]);
  portAI = atoi(argv[3]);

  ////////////// CONNECT TO C SERVER ////////////
  sockC = socketClient(nomMachServ, portC);
  printf("(Client) Connected to C Server!\n");

  ////////////// CONNECT TO JAVA SERVER ////////////
  sockAI = socketClient(nomMachServ, portAI);
  printf("(Client) Connected to JAVA Server!\n");

  /////////////// GET NAME ///////////
  printf("(Client) To play, type the player name!\n");
  scanf("%s", chaine);

  ///////// ASK FOR PARTICIPATION //////////
  TPartieReq participationReq;
  strcpy(participationReq.nomJoueur, chaine);
  participationReq.idRequest = PARTIE;
  send(sockC, &participationReq, sizeof(TPartieReq), 0);
  printf("(Client) Participation request sent in name of %s!\n", participationReq.nomJoueur);

  ///////// RECIEIVE PARTICIPATION ACK //////////
  TPartieRep participationRes;
  recv(sockC, &participationRes, sizeof(TPartieRep), 0);
  playerColor = handleParticipationAck(participationRes);

  /////////// PLAYS START ////////////////
  while (matchNumber < NUM_OF_MATCHES)
  {
    printf("\n");
    printf("(Client) Match number %d is about to START!\n", matchNumber);
    printf("\n");

    // Do first play of the match
    if ((playerColor == BLANC && matchNumber == 0 && matchIsOn) ||
        (playerColor == NOIR && matchNumber == 1 && matchIsOn))
    {
      matchIsOn = makeMove(playerColor, sockAI, sockC);
    }
    // Start to send plays
    while (matchIsOn)
    {
      // Receive & Treat Oponent Play Validation
      TCoupRep playRes2;
      recv(sockC, &playRes2, sizeof(TCoupRep), 0);
      matchIsOn = handleOponentPlayValidation(playRes2);

      // Make new play
      if (matchIsOn)
      {
        matchIsOn = makeMove(playerColor, sockAI, sockC);
      }
    }

    // Prepare for next match
    matchIsOn = true;
    matchNumber++;
  }

  printf("GAME FINISHED, TURNING OFF THE SERVER!!!\n");
  close(sockC);
  return 0;
}