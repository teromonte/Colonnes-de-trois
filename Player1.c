#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "fonctionsTCP.h"
#include "javaServerAPI.h"

#define TAIL_BUF 20

int main(int argc, char **argv)
{
  int sock, portSend, err;
  char *nomMachServ;

  struct request;
  struct pair;

  /* verification des arguments */
  if (argc != 3)
  {
    printf("usage : %s nom/IPServ portSend \n", argv[0]);
    return -1;
  }

  nomMachServ = argv[1];
  portSend = atoi(argv[2]);

  sock = socketClient(nomMachServ, portSend);
  if (sock < 0)
  {
    perror("(clientTCP) sur socketClient\n");
    close(sock);
    return -2;
  }

  int i = 0;
  while (i < 10)
  {
    int c = '2';

    err = send(sock, &c, sizeof(int), 0);
    err = send(sock, &c, sizeof(int), 0);
    err = send(sock, &c, sizeof(int), 0);
    if (err <= 0)
    {
      perror("(client) erreur sur le send\n");
      shutdown(sock, SHUT_RDWR);
      close(sock);
      return -3;
    }

    int res;

    err = recv(sock, &res, sizeof(int), 0);
    if (err <= 0)
    {
      perror("(client) erreur sur le receive\n");
      shutdown(sock, SHUT_RDWR);
      close(sock);
      return -4;
    }

    printf("response: %c\n", res);

    sleep(5);
    i++;
  }

  shutdown(sock, SHUT_RDWR);
  close(sock);

  return 0;
}