#include "fonctionsTCP.h"
#include "protocolColonne.h"


int main(int argc, char **argv) {

  int sock,                /* descripteur de la socket locale */
      port;                /* variables de lecture */
  char* nomMachServ;       /* pour solution getaddrinfo */

  int err;
  int choix = 1;

 char chaine[100];
  char dem ;

  TPartieReq req;
  TPartieRep rep;
 TCoupReq Creq ;
 TCoupReq Creq2 ;
 TCoupRep Crep ;
 TCoupRep Crep2 ;
 TCase tcase ;
 TCase tcase2 ;


  /* verification des arguments */
  if (argc != 3) {
    printf("usage : %s nom/IPServ port\n", argv[0]);
    return -1;
  }

  nomMachServ = argv[1];

  port = atoi(argv[2]);

  sock = socketClient(nomMachServ,port);

  while(choix == 1)
  {

printf("Pour participer tapez p et envoyez votre login \n");



 printf("(client) tapez p pour participer ainsi votre nom ou login : ");
  scanf("%c %s" , &dem , chaine);
  printf("(client) envoi de - %s - \n", chaine);




  switch(dem)
    {
      case 'p':
          req.idRequest = PARTIE;
          break;
      case '.' :
            break;
    }


     memcpy(req.nomJoueur, chaine,strlen(chaine));
    printf("(client) send nom est %s \n", req.nomJoueur);

    err = send(sock, &req, sizeof(TPartieReq), 0);
  if (err <= 0) {
    perror("(client) erreur sur le send");
    shutdown(sock, SHUT_RDWR); close(sock);
  }

    printf("(client) send demande parti OK \n");


    /*
    * reception et affichage du message en provenance du Serveur
    */

    err = recv(sock, &rep, sizeof(TPartieRep), 0);
    if (err <= 0) {
      perror("(Client) erreur dans la reception");
      close(sock);
      return -6;
    }

    switch(rep.err)
    {
      case ERR_OK:
          if(rep.coul == BLANC){
             printf("(Client) vous etes le jouer blanc et le nom de votre adverse c'est : %s\n", rep.nomAdvers);
               break;
         }

         if(rep.coul == NOIR){
               printf("(Client) vous etes le jouer noir et le nom de votre adverse c'est : %s\n", rep.nomAdvers);
                 break;
         }

      case ERR_PARTIE:
          printf("pas possible de participer !\n");
          break;
      default:
          break;
    }





choix = 2 ;

printf("\n");

  }



    while(choix == 2)
  {


     if(rep.coul == BLANC){
          printf("(Client) you are the first player! lancer un coup\n");
          // TODO ask the IA for a coup and get the result

         /* Creq.idRequest = COUP;
          Creq.typeCoup = POS_PION;
          Creq.coul = BLANC ;
          tcase.lg = TROIS ;
          tcase.col = B ;
          Creq.action.posPion = tcase ;*/

          err = send(sock, &Creq, sizeof(TCoupReq), 0);
  if (err <= 0) {
    perror("(client) erreur sur le send de coup ");
    shutdown(sock, SHUT_RDWR); close(sock);
  }

    printf("(client) send demande coup de jouer 1 OK \n");


       err = recv(sock, &Crep, sizeof(TCoupRep), 0);
    if (err <= 0) {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sock);
      return -20;
    }

    switch(Crep.err)
    {
      case ERR_OK:
          if(rep.coul == BLANC){
             if(Crep.validCoup == VALID){
          printf("(Client) coup bien jouee!! \n" );
                if(Crep.propCoup == CONT){
          printf("(Client) votre coup est valid et aucun gagne pour le moment! \n");


                  }
                  if(Crep.propCoup == GAGNE){
                            printf("(Client) you are wenning! \n" );
                  }

               }


 break;
         }

break;


      case ERR_COUP:
      printf("tu ne peut pas jouer un coup avant de participer !\n");
          break;
       default:
           break ;
    }


err = recv(sock, &Crep2, sizeof(TCoupRep), 0);
    if (err <= 0) {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sock);
      return -20;
    }


 switch(Crep2.err)
    {
      case ERR_OK:
             if(Crep2.validCoup == VALID){
          printf("(Client) coup adverse bien jouee!! \n" );
             if(Creq2.typeCoup == POS_PION){
          printf("(Client) il a fait un pos  \n");
          }
                if(Crep2.propCoup == CONT){
          printf("(Client) aucun gagne pour le moment! \n");


                  }
                  if(Crep.propCoup == GAGNE){
                            printf("(Client) the adverse is wenning! \n" );
                  }

               }



break;


      case ERR_COUP:
      printf("tu ne peut pas jouer un coup avant de participer !\n");
          break;
      default:
          break;
    }


 break;
         }




         if(rep.coul == NOIR){
          printf("(Client) you are the second player , wait! \n");

     err = recv(sock, &Crep, sizeof(TCoupRep), 0);
    if (err <= 0) {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sock);
      return -20;
    }

    switch(Crep.err)
    {
      case ERR_OK:
          if(rep.coul == NOIR){
             if(Crep.validCoup == VALID){
          printf("(Client) the first player send this coup bien jouee!! \n" );
          if(Creq.typeCoup == POS_PION){
          printf("(Client) il a fait un pos  \n");
          }

                if(Crep.propCoup == CONT){
          printf("(Client) et aucun gagne pour le moment! \n");


                  }
                  if(Crep.propCoup == GAGNE){
                            printf("(Client) you are wenning! \n" );
                  }


        // TODO send the adverse coup //
        /*  Creq2.idRequest = COUP;
          Creq2.typeCoup = POS_PION;
          Creq2.coul = NOIR ;
          tcase2.lg = DEUX ;
          tcase2.col = C;
          Creq2.action.posPion = tcase2;*/

          err = send(sock, &Creq2, sizeof(TCoupReq), 0);
  if (err <= 0) {
    perror("(client) erreur sur le send de coup ");
    shutdown(sock, SHUT_RDWR); close(sock);
  }

    printf("(client) send demande coup de seconde player pouer OK \n");


     err = recv(sock, &Crep2, sizeof(TCoupRep), 0);
    if (err <= 0) {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sock);
      return -20;
    }


    switch(Crep2.err)
    {
      case ERR_OK:
             if(Crep2.validCoup == VALID){
          printf("(Client) this coup bien jouee!! \n" );
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




               }


 break;
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

 break;



 break;
         }






//choix = 2 ;

printf("\n");

  }




  close(sock);

  return 0;
}