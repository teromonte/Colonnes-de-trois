#include "fonctionsTCP.h"
#include "protocolColonne.h"
#define MAX_CLIENT 2
#define MAXB 30

#include <stdbool.h>




int main(int argc, char** argv) {
  int  sockConx,        /* descripteur socket connexion */
       port,            /* numero de port */
       sizeAddr        /* taille de l'adresse d'une socket */
       ;


  int sockTrans[MAX_CLIENT];
  int acc = 0 ;




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
       for(int k = 0 ; k < MAXB ; k++){
                nom1[k] = Preq.nomJoueur[k] ;
       }

       //err = recv(sockTrans[i],nom1,sizeof(Preq.nomJoueur), 0);
       printf("(serveurTCP) voila le nom recu : %s\n", nom1);
       //memcpy( Preq.nomJoueur, nom1 , sizeof nom1);
       // int size1 = strlen(nom2);
       //   printf("(serveur) erreur recu complet de parti de client %d  et de taille %c \n" , i , nom1);
       }

       if(i == 1){
       for(int k = 0 ; k < MAXB ; k++){
                nom2[k] = Preq.nomJoueur[k] ;
       }

        //err = recv(sockTrans[i],nom2,MAXB, 0);
       printf("(serveurTCP) voila le nom recu : %s\n", nom2);
       //memcpy( Preq.nomJoueur, nom2 , sizeof nom2);
       // int size1 = strlen(nom2);
         // printf("(serveur) erreur recu complet de parti de client %d  et de taille %c \n" , i , nom2);
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

         for(int k = 0 ; k < MAXB ; k++){
                Prep.nomAdvers[k] = nom2[k];
       }

      printf("(serveur) le nom adverse est %s\n" , Prep.nomAdvers);
         //Prep.nomAdvers[TNOM] = Preq.nomJoueur[TNOM] ;
       err = send(sockTrans[j], &Prep, sizeof(TPartieRep), 0);
        if( err > 0)
       printf("(serveur) send complet de demande de parti de client %d \n" , j);
         j++;


       }

          if(j == 1) {
         Prep.err = ERR_OK ;
         Prep.coul = NOIR ;
         for(int k = 0 ; k < MAXB ; k++){
                Prep.nomAdvers[k] = nom1[k];
       }
      printf("(serveur) le nom adverse est %s\n" , Prep.nomAdvers);
         //Prep.nomAdvers[TNOM] = Preq.nomJoueur[TNOM] ;
       err = send(sockTrans[j], &Prep, sizeof(TPartieRep), 0);
        if( err > 0){
       printf("(serveur) send complet de demande de parti de client %d \n" , j);
           }
       else{
          printf("(serveur) send recu complet de parti de client %d \n" , j);
       }
              //err = send(sockTrans[i], &Prep, sizeof(TPartieRep), 0);


    }

     j++;





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

