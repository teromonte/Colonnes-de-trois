#include "../headers/functionsServer.h"

int fun2(TPion color, int sockAI, stru2 *res)
{
  int err;
  int moveType;
  int depCol, depLg, arrCol, arrLg;

  err = send(sockAI, &color, sizeof(int), 0);

  err = recv(sockAI, &moveType, sizeof(int), 0);
  err = recv(sockAI, &depCol, sizeof(int), 0);
  err = recv(sockAI, &depLg, sizeof(int), 0);
  err = recv(sockAI, &arrCol, sizeof(int), 0);
  err = recv(sockAI, &arrLg, sizeof(int), 0);

  TCase dep = {.col = depCol, .lg = depLg};
  TCase arr = {.col = arrCol, .lg = arrLg};
  TDeplPion displace = {.caseDep = dep, .caseArr = arr};

  res->typeMove = moveType;
  res->placeMove = dep;
  res->displaceMove = displace;

  return err;
}
