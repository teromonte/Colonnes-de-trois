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

#define TAIL_BUF 20
int main(int argc, char **argv)
{
  int sock,          /* descripteur de la socket locale */
      port,          /* variables de lecture */
      err;           /* code d'erreur */
  float resultat;    /* message */
  char *ipMachServ;  /* pour solution inet_aton */
  char *nomMachServ; /* pour solution getaddrinfo */

  /* verification des arguments */
  if (argc != 3)
  {
    printf("usage : %s nom/IPServ port\n", argv[0]);
    return -1;
  }

  ipMachServ = argv[1];
  nomMachServ = argv[1];
  port = atoi(argv[2]);

  // Utilisation de fonctionsTCP.c
  sock = socketClient(nomMachServ, port);

  do
  {
    /*
     * envoi de la chaine
     */
    err = send(sock, &op, sizeof(struct operation), 0);
    if (err <= 0)
    { // if (err != strlen(chaine)+1) {
      perror("(client) erreur sur le send");
      shutdown(sock, SHUT_RDWR);
      close(sock);
      return -5;
    }
    printf("(client) envoi de l'opération\n");

    err = recv(sock, &resultat, sizeof(float), 0);
    if (err <= 0)
    { // if (err != strlen(chaine)+1) {
      perror("(client) erreur sur le send\n");
      shutdown(sock, SHUT_RDWR);
      close(sock);
      return -4;
    }
    printf("(client) réception de %f \n", resultat);
  } while (1);

  /*
   * fermeture de la connexion et de la socket
   */
  shutdown(sock, SHUT_RDWR);
  close(sock);

  return 0;
}
