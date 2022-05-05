#include "fonctionsTCP.h"
#include "protocolColonne.h"
#include "validation.h"
#include <stdbool.h>
#define MAX_CLIENT 2
#define MAXB 30






int main(int argc, char** argv) {
  int  sockConx,        /* descripteur socket connexion */
       port,            /* numero de port */
       sizeAddr        /* taille de l'adresse d'une socket */
       ;


  int sockTrans[MAX_CLIENT];
  int acc = 0 ;


int nbcJ1 = 0;
int nbcJ2 = 0 ;

   /*
  char operateur;
  float operande1;
  float operande2;
  */


  struct sockaddr_in addClient;    /* adresse de la socket client connectee */

 // float resultat;


  /*
   * verification des arguments
   */
  if (argc != 2) {
    printf ("usage : %s port\n", argv[0]);
    return -1;
  }

  port  = atoi(argv[1]);

  sockConx = socketServeur(port);

  sizeAddr = sizeof(struct sockaddr_in);

 char nom1[MAXB];
 char nom2[MAXB];

 TIdRequest Idreq ;
  TPartieReq Preq;
  TPartieRep Prep;

  bool gameon = true ;
  while(gameon)
  {

  int err ;
     /*
     * attente de connexion
     */


while(acc < MAX_CLIENT){

      sockTrans[acc] = accept(sockConx, (struct sockaddr *)&addClient, (socklen_t *)&sizeAddr);
      if (sockTrans[acc] < 0) {
        perror("(serveurTCP) erreur sur accept");
        return -5;
      }

acc++;

printf("acc = %d \n" , acc);
 }

 int i = 0 ;
    while ( i < MAX_CLIENT ){
              err = recv(sockTrans[i], &Idreq,sizeof(TIdRequest), MSG_PEEK);
              if(err > 0){
              printf("(serveur) recu demande parti OK  de client %d \n" , i);
}
 switch(Idreq){
 case PARTIE :
        err = recv(sockTrans[i],&Preq,sizeof(TPartieReq), 0);


        if( err > 0){
       printf("(serveur) recu complet de demande de parti de client %d \n" , i);
           }
       else{
          printf("(serveur) erreur recu complet de parti de client %d \n" , i);
       }

       if(i ==0){


       memcpy( nom1, Preq.nomJoueur , sizeof nom1);
       printf("(serveurTCP) voila le nom recu : %s\n", nom1);
       }

       if(i == 1){


       memcpy( nom2, Preq.nomJoueur , sizeof nom2);
              printf("(serveurTCP) voila le nom recu : %s\n", nom2);


            }


            break;
          case COUP :
            break;
        }
        i++;
    }



   int j = 0 ;
 while( j < MAX_CLIENT){

   if(j == 0){

       printf("(serveur) waitt !%d \n" , j);
         Prep.err = ERR_OK ;
         Prep.coul = BLANC ;

        memcpy(Prep.nomAdvers, nom2 , sizeof Prep.nomAdvers);

      printf("(serveur) le nom adverse est %s\n" , Prep.nomAdvers);
       err = send(sockTrans[j], &Prep, sizeof(TPartieRep), 0);
        if( err > 0)
       printf("(serveur) send complet de demande de parti de client %d \n" , j);
         j++;


       }

          if(j == 1) {
         Prep.err = ERR_OK ;
         Prep.coul = NOIR ;
         memcpy(Prep.nomAdvers, nom1 , sizeof Prep.nomAdvers);
      printf("(serveur) le nom adverse est %s\n" , Prep.nomAdvers);
       err = send(sockTrans[j], &Prep, sizeof(TPartieRep), 0);
        if( err > 0){
       printf("(serveur) send complet de demande de parti de client %d \n" , j);
           }
       else{
          printf("(serveur) send recu complet de parti de client %d \n" , j);
       }


    }

     j++;


  }

initialiserPartie();

     while (nbcJ1 < 20 && nbcJ2 < 20) {

      TCoupReq Creq;
      TCoupRep Crep;

        printf("(serveur) Attente de la reception de la requete COUP du J1\n");
        err = recv(sockTrans[0], &Creq, sizeof(TCoupReq), 0);
        if (err < 0) {
            perror("(serveur) erreur sur la reception de la requete COUP du J1 ");
            return -10;
        }
        nbcJ1++;
        bool verif;

        TPropCoup propCoupJ1;
        verif = validationCoup(1, Creq, &propCoupJ1);

        /*
        * Creation réponse COUP du J1
        */
        Crep.err = ERR_OK;
        Crep.propCoup = propCoupJ1;
        if (verif) {
            Crep.validCoup = VALID;
        } else {
            Crep.validCoup = TRICHE;
        }

        /*
        * Envoie réponse COUP du J1 a J1
        */
        printf("(serveur) Envoie de la reponse COUP a J1\n");
        err = send(sockTrans[0], &Crep, sizeof(TCoupRep), 0);
        if (err < 0) {
            perror("(serveur) erreur sur l'envoi de la reponse COUP a J1 ");
            return -11;
        }

        /*
        * Envoie réponse COUP du J1 a J2
        */
        printf("(serveur) Envoie de la reponse COUP a J2\n");
        err = send(sockTrans[1], &Crep, sizeof(TCoupRep), 0);
        if (err < 0) {
            perror("(serveur) erreur sur l'envoi de la reponse COUP a J2 ");
            return -12;
        }

       switch (Crep.propCoup){
        case NULLE :
            printf("(serveur) resultats est egeau\n");
        break;

        case GAGNE :
            printf("(serveur) the first player gagne\n");
        break;

        case PERDU :
            printf("(serveur) the seconde player gagne\n");
        break;


        case CONT :
            printf("(serveur) no one won the game!!\n");
        break;

        }



        //--------------------------------- J2 ------------------------------------------------------------------------------------------------

        /*
        * Attente de la requete COUP du J2
        */

        printf("(serveur) Attente de la reception de la requete COUP du J2\n");
        err = recv(sockTrans[1], &Creq, sizeof(TCoupReq), 0);
        if (err < 0) {
            perror("(serveur) erreur sur la reception de la requete COUP du J2 ");
            return -14;
        }
        nbcJ2++;


        TPropCoup propCoupJ2;
        verif = validationCoup(2, Creq, &propCoupJ2);


        if( verif){         printf("true\n");}
        else{        printf("false\n");}

        /*
        * Creation réponse COUP du J2
        */

        Crep.err = ERR_OK;
        Crep.propCoup = propCoupJ2;
        if (verif) {
            Crep.validCoup = VALID;
        } else {
            Crep.validCoup = TRICHE;
        }


        /*
        * Envoie réponse COUP du J2 a J2
        */
        printf("(serveur) Envoie de la reponse COUP a J2\n");
        err = send(sockTrans[1], &Crep, sizeof(TCoupRep), 0);
        if (err < 0) {
            perror("(serveurNewton) erreur sur l'envoi de la reponse COUP a J2 ");
            return -15;
        }

        /*
        * Envoie réponse COUP du J2 a J1
        */
        printf("(serveur) Envoie de la reponse COUP a J1\n");
        err = send(sockTrans[0], &Crep, sizeof(TCoupRep), 0);
        if (err < 0) {
            perror("(serveur) erreur sur l'envoi de la reponse COUP a J1");
            return -16;
        }

      switch (Crep.propCoup){
        case NULLE :
            printf("(serveur) resultats est egeau\n");
        break;

        case GAGNE :
            printf("(serveur) the first player gagne\n");
        break;

        case PERDU :
            printf("(serveur) the seconde player gagne\n");
        break;



        case CONT :
            printf("(serveur) no one won the game!!\n");
        break;

        }



        /*
         * Envoie requete COUP du J2 a J1
         */
        printf("(serveur) Envoie de la requete COUP a J1\n");
        err = send(sockTrans[0], &Crep, sizeof(TCoupReq), 0);
        if (err < 0) {
            perror("(serveurNewton) erreur sur l'envoi de la requete COUP a J1");
            return -17;
        }

}





printf("\n");

  }


  /*
   * arret de la connexion et fermeture
   */
  for( int i = 0 ; i < MAX_CLIENT ; i++){
    shutdown(sockTrans[i], SHUT_RDWR);
    close(sockTrans[i]);
    close(sockConx);
  }

  return 0;
}
