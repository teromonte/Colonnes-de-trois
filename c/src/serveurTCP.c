#include "../header/protocolColonne.h"
#include "../lib/headers/fonctionsTCP.h"
#include "../lib/headers/validation.h"

#include <stdbool.h>
#define MAX_CLIENT 2
#define MAXB 30

int main(int argc, char **argv)
{
  int sockConx, /* descripteur socket connexion */
      port,     /* numero de port */
      sizeAddr; /* taille de l'adresse d'une socket */

  int sockTrans[MAX_CLIENT];
  int acc = 0;

  int nPlays0 = 0;
  int nPlays1 = 0;

  struct sockaddr_in addClient; /* adresse de la socket client connectee */

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
  sizeAddr = sizeof(struct sockaddr_in);

  char nom1[MAXB];
  char nom2[MAXB];

  TIdRequest Idreq;
  TPartieReq Preq;
  TPartieRep Prep;

  bool gameon = true;
  while (gameon)
  {
    // attente de connexion
    int err;
    while (acc < MAX_CLIENT)
    {

      sockTrans[acc] = accept(sockConx, (struct sockaddr *)&addClient, (socklen_t *)&sizeAddr);
      if (sockTrans[acc] < 0)
      {
        perror("(serveurTCP) erreur sur accept");
        return -5;
      }

      acc++;

      printf("acc = %d \n", acc);
    }
    printf("(serveur) exit attente de connexion\n");
    // Receive of participate
    int i = 0;
    while (i < MAX_CLIENT)
    {
      err = recv(sockTrans[i], &Idreq, sizeof(TIdRequest), MSG_PEEK);
      if (err > 0)
      {
        printf("(serveur) recu demande parti OK de client %d \n", i);
      }
      switch (Idreq)
      {
      case PARTIE:
        err = recv(sockTrans[i], &Preq, sizeof(TPartieReq), 0);

        if (err > 0)
        {
          printf("(serveur) recu complet de demande de parti de client %d \n", i);
        }
        else
        {
          printf("(serveur) erreur recu complet de parti de client %d \n", i);
        }

        if (i == 0)
        {

          memcpy(nom1, Preq.nomJoueur, sizeof nom1);
          printf("(serveurTCP) voila le nom recu : %s\n", nom1);
        }

        if (i == 1)
        {

          memcpy(nom2, Preq.nomJoueur, sizeof nom2);
          printf("(serveurTCP) voila le nom recu : %s\n", nom2);
        }

        break;
      case COUP:
        break;
      }
      i++;
    }
    printf("(serveur) exit receive of participate\n");
    // Response to the participants
    int j = 0;
    while (j < MAX_CLIENT)
    {

      if (j == 0)
      {

        printf("(serveur) waitt !%d \n", j);
        Prep.err = ERR_OK;
        Prep.coul = BLANC;

        memcpy(Prep.nomAdvers, nom2, sizeof Prep.nomAdvers);

        printf("(serveur) le nom adverse est %s\n", Prep.nomAdvers);
        err = send(sockTrans[j], &Prep, sizeof(TPartieRep), 0);
        if (err > 0)
          printf("(serveur) send complet de demande de parti de client %d \n", j);
        j++;
      }

      if (j == 1)
      {
        Prep.err = ERR_OK;
        Prep.coul = NOIR;
        memcpy(Prep.nomAdvers, nom1, sizeof Prep.nomAdvers);
        printf("(serveur) le nom adverse est %s\n", Prep.nomAdvers);
        err = send(sockTrans[j], &Prep, sizeof(TPartieRep), 0);
        if (err > 0)
        {
          printf("(serveur) send complet de demande de parti de client %d \n", j);
        }
        else
        {
          printf("(serveur) send recu complet de parti de client %d \n", j);
        }
      }

      j++;
    }
    printf("(serveur) exit Response to the participants\n");

    // Start Game
    int match = 0;
    int turn = 0;
    printf("(serveur) initialiserPartie\n");
    initialiserPartie();
    while (match < 2) // server 
    {
      while (nPlays0 < 20 && nPlays1 < 20)
      {
        // receive and response
        TCoupReq request1;
        TCoupRep response1;
        TPropCoup propCoup;
        bool verif;

        // receive coup
        printf("(serveur) Attente de la reception de la requete COUP du J1\n");
        err = recv(sockTrans[turn], &request1, sizeof(TCoupReq), 0);
        if (err < 0)
        {
          perror("(serveur) erreur sur la reception de la requete COUP du J1 ");
          return -10;
        }

        // validate coup
        verif = validationCoup(turn + 1, request1, &propCoup);
        printf("(serveur) Just verified pos\n");
        // Create response Crep COUP du J1
        response1.err = ERR_OK;
        response1.propCoup = propCoup;

        switch (response1.propCoup)
        {
        case NULLE:
          printf("(serveur) resultats est egeau\n");
          response1.validCoup = TIMEOUT;
          break;

        case GAGNE:
          printf("(serveur) the first player gagne\n");
          response1.validCoup = VALID;
          match++;
          break;

        case PERDU:
          printf("(serveur) the seconde player gagne\n");
          response1.validCoup = TRICHE;
          match++;
          break;

        case CONT:
          printf("(serveur) no one won the game!!\n");
          response1.validCoup = VALID;
          break;
        }

        // send response to players
        for (int i = 0; i < 2; i++)
        {
          printf("(serveur) Envoie de la reponse COUP\n");
          err = send(sockTrans[i], &response1, sizeof(TCoupRep), 0);
          if (err < 0)
          {
            perror("(serveur) erreur sur l'envoi de la reponse COUP a J" + i);
            return -11;
          }
        }

        if (turn == 0)
        {
          nPlays0++;
          turn = 1;
        }
        else
        {
          nPlays1++;
          turn = 0;
        }
      }
      turn = 1;
    }
    int n = 0;
    printf("\n");
  }

  /*
   * arret de la connexion et fermeture
   */
  for (int i = 0; i < MAX_CLIENT; i++)
  {
    shutdown(sockTrans[i], SHUT_RDWR);
    close(sockTrans[i]);
    close(sockConx);
  }

  return 0;
}
