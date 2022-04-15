#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "fonctionsTCP.h"

/* taille du buffer de reception */
#define TAIL_BUF 20

int main(int argc, char **argv)
{
    int sockConx,  /* descripteur socket connexion */
        sockTrans, /* descripteur socket transmission */
        port,      /* numero de port */
        sizeAddr,  /* taille de l'adresse d'une socket */
        err;       /* code d'erreur */
    float resultat;        /* message */
    char buffer[TAIL_BUF]; /* buffer de reception */

    struct sockaddr_in addClient; // adresse de la socket client connectee

    /*
     * verification des arguments
     */
    if (argc != 2)
    {
        printf("usage : %s port\n", argv[0]);
        return -1;
    }

    port = atoi(argv[1]);

    sockConx = socketServeur(port);
    if (sockConx < 0)
    {
        perror("(serveurTCP) sur socketServer\n");
        close(sockConx);
        return -2;
    }

    do
    {
        sizeAddr = sizeof(struct sockaddr_in);
        sockTrans = accept(sockConx, (struct sockaddr *)&addClient, (socklen_t *)&sizeAddr);
        if (sockTrans < 0)
        {
            perror("(serveurTCP) erreur sur accept\n");
            close(sockTrans);
            close(sockConx);
            return -3;
        }

        /*
         * reception et affichage du message en provenance du client
         */
        err = recv(sockTrans, &op, sizeof(struct operation), 0);
        if (err <= 0)
        {
            perror("(serveurTCP) erreur dans la reception\n");
            shutdown(sockTrans, SHUT_RDWR);
            close(sockTrans);
            close(sockConx);
            return -4;
        }
        printf("(serveurTCP) reception de l'operation\n");

        err = operation(op.opt, op.opd1, op.opd2, &resultat);
        if (err < 0)
        {
            perror("(serveur) erreur sur le calcul de l'operation\n");
            shutdown(sockTrans, SHUT_RDWR);
            close(sockTrans);
            close(sockConx);
            return -5;
        }

        err = send(sockTrans, &resultat, sizeof(float), 0);
        if (err <= 0)
        { // if (err != strlen(chaine)+1) {
            perror("(serveur) erreur sur le send\n");
            shutdown(sockTrans, SHUT_RDWR);
            close(sockTrans);
            close(sockConx);
            return -6;
        }
        printf("(serveur) envoi du résultat %f realise\n", resultat);

    } while (1);

    /*
     * fermeture de la connexion et de la socket
     */
    shutdown(sockTrans, SHUT_RDWR);
    close(sockTrans);
    close(sockConx);

    return 0;
}
