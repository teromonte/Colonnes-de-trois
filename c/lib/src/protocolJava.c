#include "../headers/protocolJava.h"

int requestAI(int color, int sockAI)
{
  int res, err;

  err = send(sockAI, &color, sizeof(int), 0);
  if (err <= 0)
  {
    perror("(client) erreur sur le send\n");
    shutdown(sockAI, SHUT_RDWR);
    close(sockAI);
    return -3;
  }

  err = recv(sockAI, &res, sizeof(int), 0);
  if (err <= 0)
  {
    perror("(client) erreur sur le receive\n");
    shutdown(sockAI, SHUT_RDWR);
    close(sockAI);
    return -4;
  }

  return res;
}
