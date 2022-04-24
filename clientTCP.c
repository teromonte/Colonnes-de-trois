#include "fonctionsTCP.h"
#include "ColonClAserv.h"
#include "protocolColonne.h"
#include <stdbool.h>

int main(int argc, char **argv)
{

  int sock,          /* descripteur de la socket locale */
      port;          /* variables de lecture */
  char *nomMachServ; /* pour solution getaddrinfo */

  int err;
  int choix = 4;

  char dem;
  // char nomJoueur[TNOM] ;

  TPartieReq req;
  TPartieRep rep;

  /* verification des arguments */
  if (argc != 3)
  {
    printf("usage : %s nom/IPServ port\n", argv[0]);
    return -1;
  }

  nomMachServ = argv[1];

  port = atoi(argv[2]);

  sock = socketClient(nomMachServ, port);

  while (choix == 4)
  {

    // printf("Pour participer tapez p et envoyez votre login \n");

    lireDemane(&dem, &req.nomJoueur[TNOM]);

    switch (dem)
    {
    case 'p':
      req.idRequest = PARTIE;
      break;
    case '.':
      break;
    }

    err = send(sock, &req, sizeof(req), 0);
    if (err <= 0)
    {
      perror("(client) erreur sur le send");
      shutdown(sock, SHUT_RDWR);
      close(sock);
    }

    printf("(client) send demande parti OK  \n");
    /*
     * reception et affichage du message en provenance du Serveur
     */
    err = recv(sock, &rep, sizeof(rep), 0);
    if (err <= 0)
    {
      perror("(Client) erreur dans la reception");
      close(sock);
      return -6;
    }

    switch (rep.err)
    {
    case ERR_OK:
      if (rep.coul == BLANC)
      {
        printf("(Client) vous etes le jouer blanc : %c\n", rep.nomAdvers[TNOM]);
      }
      else
      {
        printf("(Client) vous etes le jouer noir : %c\n", rep.nomAdvers[TNOM]);
      }
      break;
    case ERR_PARTIE:
      printf("pas possible de participer !\n");
      break;
    case ERR_COUP:
      printf("tu ne peut pas jouer un coup avant de participer !\n");
      break;
    case ERR_TYP:
      printf("Erreur dans le type de requete\n");
      break;
    }

    printf("Pour jouer un coup tapez 1  \nPour quitter selectionner 2\n");

    printf("Votre Choix : ");

    scanf("%d", &choix);

    printf("\n");
  }

  close(sock);

  return 0;
}
