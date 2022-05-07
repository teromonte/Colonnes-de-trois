#include "../headers/protocolJava.h"

int requestAI(TPion color, int sockAI, responseAI *res)
{
  res->color = 56;
  res->typeMove = 1;
  TCase place = {.col = 1, .lg = 1};
  res->placeMove = place;
  TDeplPion discplace = {.caseArr = place, .caseDep = place};
  res->displaceMove = discplace;
  /*
  output:
    TCoup ->   POS_PION, DEPL_PION, PASSE
    option:
      TCase = TCol + TLigne -> (  UN, DEUX, TROIS) + (  A, B, C)
      TDeplPion = TCase + TCase -> [(UN,DEUX,TROIS)+(A,B,C)]+[(UN,DEUX,TROIS)+(A,B,C)]
*/
  int res1, err;

  err = send(sockAI, &color, sizeof(int), 0);
  err = send(sockAI, &color, sizeof(int), 0);
  err = send(sockAI, &color, sizeof(int), 0);
  if (err <= 0)
  {
    perror("(client) erreur sur le send\n");
    shutdown(sockAI, SHUT_RDWR);
    close(sockAI);
  }

  err = recv(sockAI, &res1, sizeof(int), 0);
  if (err <= 0)
  {
    perror("(client) erreur sur le receive\n");
    shutdown(sockAI, SHUT_RDWR);
    close(sockAI);
  }

  return err;
}
