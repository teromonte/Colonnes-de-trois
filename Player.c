#include "fonctionsTCP.h"
#include "protocolJava.h"

#define TAIL_BUF 20
/*
#ifndef CST
#define CST
    // def str (fct-proto)
#endif
*/

int main(int argc, char **argv)
{
  char chaine[TAIL_BUF]; /* buffer */
  float resultat;        /* message */
  int sock,              /* descripteur de la socket locale */
      portSend,
      err;           /* code d'erreur */
  char *ipMachServ;  /* pour solution inet_aton */
  char *nomMachServ; /* pour solution getaddrinfo */
  struct operation op;

  /* verification des arguments */
  if (argc != 3)
  {
    printf("usage : %s nom/IPServ portSend \n", argv[0]);
    return -1;
  }
  ipMachServ = argv[1];
  nomMachServ = argv[1];
  portSend = atoi(argv[2]);

  sock = socketClient(nomMachServ, portSend);
  if (sock < 0)
  {
    perror("(clientTCP) sur socketClient\n");
    close(sock);
    return -2;
  }

  do
  {

    lireOperation(&op.opt, &op.opd1, &op.opd2);

    /*
     * envoi de la chaine
     */
    int tmpOp = op.opt;
    err = send(sock, &tmpOp, sizeof(int), 0);
    err = send(sock, &op.opd1, sizeof(float), 0);
    err = send(sock, &op.opd2, sizeof(float), 0);
    if (err <= 0)
    { // if (err != strlen(chaine)+1) {
      perror("(client) erreur sur le send\n");
      shutdown(sock, SHUT_RDWR);
      close(sock);
      return -3;
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
  } while (err > 0);
  /*
   * fermeture de la connexion et de la socket
   */
  shutdown(sock, SHUT_RDWR);
  close(sock);

  return 0;
}