#include "../lib/headers/protocolJava.h"
#include "../header/protocolColonne.h"
#include "../lib/headers/fonctionsTCP.h"

int main(int argc, char **argv)
{

  int sockC,
      portC,  /* variables de lecture */
      sockAI, /* descripteur de la socket locale */
      portAI,
      err,
      playerColor;

  char chaine[TNOM], dem, *nomMachServ;

  TPartieReq participationReq; // Participation request
  TPartieRep participationRep; // Answer to the participation request
  TCoupReq playReq;            // Play request
  TCoupRep ownPlayRes;         // Response to the play request
  TCoupRep opponentPlayRes;
  responseAI javaAPIRes;

  // verification des arguments
  if (argc != 4)
  {
    printf("usage : %s nom/IPServ portC portAI\n", argv[0]);
    return -1;
  }

  nomMachServ = argv[1];
  portC = atoi(argv[2]);
  portAI = atoi(argv[3]);

  ////////////// CONNECT TO C SERVER ////////////

  sockC = socketClient(nomMachServ, portC);

  /////////////// GET NAME ///////////

  printf("(client) To play use: p playername\n");
  scanf("%c %s", &dem, chaine);
  switch (dem)
  {
  case 'p':
    participationReq.idRequest = PARTIE;
    break;
  default:
    printf("(client) wrong command %c\n", dem);
    return -1;
    break;
  }

  ///////// ASK FOR PARTICIPATION //////////

  // Requesting to play
  strncpy(participationReq.nomJoueur, chaine, TNOM);
  printf("(client) send name: '%s'\n", participationReq.nomJoueur);
  err = send(sockC, &participationReq, sizeof(TPartieReq), 0);
  if (err <= 0)
  {
    perror("(client) erreur sur le send");
    shutdown(sockC, SHUT_RDWR);
    close(sockC);
  }

  // Receive anwser from server
  err = recv(sockC, &participationRep, sizeof(TPartieRep), 0);
  if (err <= 0)
  {
    perror("(Client) Error receiving participation response");
    return -6;
  }
  switch (participationRep.err)
  {
  case ERR_OK:
    if (participationRep.coul == BLANC)
    {
      printf("(Client) vous etes le jouer blanc et le nom de votre adverse c'est : %s\n", participationRep.nomAdvers);
    }
    else if (participationRep.coul == NOIR)
    {
      printf("(Client) vous etes le jouer noir et le nom de votre adverse c'est : %s\n", participationRep.nomAdvers);
    }
    playerColor = participationRep.coul;
    break;
  case ERR_PARTIE:
    printf("pas possible de participer !\n");
    break;
  default:
    break;
  }

  printf("FINISHED BUROCRACY\n");
  printf("\n");

  /////////// PLAYS START ////////////////

  if (participationRep.coul == BLANC)
  {
    printf("(Client) you are the first player! lancer un coup\n");

    printf("Client) Trying to reach java API: %s\n", nomMachServ);
    sockAI = socketClient(nomMachServ, portAI);
    err = requestAI(playerColor, sockAI, &javaAPIRes);
    if (err <= 0)
    {
      perror("(client) javaAPI problem!");
      shutdown(sockAI, SHUT_RDWR);
      close(sockAI);
    }
    printf("Received from API: type %d placeMove col %d placeMove lg %d displaceMove col %d displaceMove lg %d\n",
           javaAPIRes.typeMove,
           javaAPIRes.placeMove.col,
           javaAPIRes.placeMove.lg,
           javaAPIRes.displaceMove.caseArr.col,
           javaAPIRes.displaceMove.caseArr.lg);

    // Arrange Play protocol package
    playReq.coul = playerColor; // color
    playReq.idRequest = COUP;   // here is always COUP (PLAY)

    // Deal with type of move
    switch (javaAPIRes.typeMove)
    {
    case POS_PION:
      playReq.typeCoup = POS_PION;
      playReq.action.posPion = javaAPIRes.placeMove;
      break;
    case DEPL_PION:
      playReq.typeCoup = DEPL_PION;
      playReq.action.deplPion = javaAPIRes.displaceMove;
      break;
    case PASSE:
      playReq.typeCoup = PASSE;
      break;
    default:
      break;
    }
    // Send play
    err = send(sockC, &playReq, sizeof(TCoupReq), 0);
    if (err <= 0)
    {
      perror("(client) erreur sur le send de coup ");
      shutdown(sockC, SHUT_RDWR);
      close(sockC);
    }
    // Receive validation of own play
    err = recv(sockC, &ownPlayRes, sizeof(TCoupRep), 0);
    if (err <= 0)
    {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sockC);
      return -20;
    }
    printf("(cclient) BLANC send play - first play!!!\n");
    printf("\n");
    // Treat Play Response
    switch (ownPlayRes.err)
    {
    case ERR_OK:
      if (ownPlayRes.validCoup == VALID)
      {
        printf("(Client) coup bien jouee!! \n");
        if (ownPlayRes.propCoup == CONT)
        {
          printf("(Client) votre coup est valid et aucun gagne pour le moment! \n");
        }
        else if (ownPlayRes.propCoup == GAGNE)
        {
          printf("(Client) you are winning! \n");
        }
      }
      else
      {
        // TODO treat TIMEOUT AND TRICHE
        printf("(Client) My own play was timeout or I was cheating! \n");
        return 1;
      }
      break;
    case ERR_COUP:
      printf("tu ne peut pas jouer un coup avant de participer !\n");
      break;
    default:
      break;
    }
  }

  // Start to send plays, runs till match is gange, perdu, triche, timeout
  while (1)
  {

    // Receive validation of adversaire play
    err = recv(sockC, &opponentPlayRes, sizeof(TCoupRep), 0);
    if (err <= 0)
    {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sockC);
      return -20;
    }
    printf("(client) Received validation of adversaire play \n");

    // Treat adversaire play
    switch (opponentPlayRes.err)
    {
    case ERR_OK:
      if (opponentPlayRes.validCoup == VALID)
      {
        if (opponentPlayRes.propCoup == CONT)
        {
          printf("(Client) aucun gagne pour le moment! \n");
        }
        else if (opponentPlayRes.propCoup == GAGNE)
        {
          printf("(Client) the adverse is wenning! \n");
        }
      }
      else
      {
        printf("(Client) Your opponent made a mistake!! \n");
        return -1;
      }
      break;
    case ERR_COUP:
      printf("tu ne peut pas jouer un coup avant de participer !\n");
      return -1;
      break;
    case ERR_TYP:
      printf("Erreur dans le type de requete\n");
      return -1;
      break;
    default:
      break;
    }
    // Arrange Play protocol package
    // TODO ask the IA for a coup and get the result

    printf("Client) Trying to reach java API: %s\n", nomMachServ);
    sockAI = socketClient(nomMachServ, portAI);
    err = requestAI(playerColor, sockAI, &javaAPIRes);
    if (err <= 0)
    {
      perror("(client) javaAPI problem");
      shutdown(sockAI, SHUT_RDWR);
      close(sockAI);
    }

    printf("Received from API: type %d placeMove col %d placeMove lg %d displaceMove col %d displaceMove lg %d\n",
           javaAPIRes.typeMove,
           javaAPIRes.placeMove.col,
           javaAPIRes.placeMove.lg,
           javaAPIRes.displaceMove.caseArr.col,
           javaAPIRes.displaceMove.caseArr.lg);

    printf("Client) Trying to reach java API: %s\n", nomMachServ);
    sockAI = socketClient(nomMachServ, portAI);
    err = requestAI(playerColor, sockAI, &javaAPIRes);
    if (err <= 0)
    {
      perror("(client) javaAPI problem!");
      shutdown(sockAI, SHUT_RDWR);
      close(sockAI);
    }
    printf("Received from API: type %d placeMove col %d placeMove lg %d displaceMove col %d displaceMove lg %d\n",
           javaAPIRes.typeMove,
           javaAPIRes.placeMove.col,
           javaAPIRes.placeMove.lg,
           javaAPIRes.displaceMove.caseArr.col,
           javaAPIRes.displaceMove.caseArr.lg);

    // Arrange Play protocol package
    playReq.coul = playerColor; // color
    playReq.idRequest = COUP;   // here is always COUP (PLAY)

    // Deal with type of move
    switch (javaAPIRes.typeMove)
    {
    case POS_PION:
      playReq.typeCoup = POS_PION;
      playReq.action.posPion = javaAPIRes.placeMove;
      break;
    case DEPL_PION:
      playReq.typeCoup = DEPL_PION;
      playReq.action.deplPion = javaAPIRes.displaceMove;
      break;
    case PASSE:
      playReq.typeCoup = PASSE;
      break;
    default:
      break;
    }

    // Send play
    err = send(sockC, &playReq, sizeof(TCoupReq), 0);
    if (err <= 0)
    {
      perror("(client) erreur sur le send de coup ");
      shutdown(sockC, SHUT_RDWR);
      close(sockC);
    }
    if (playReq.coul == BLANC)
      printf("(client) BLANC send play\n");
    else
      printf("(client) NOIR send play \n");

    // Receive validation of own play
    err = recv(sockC, &ownPlayRes, sizeof(TCoupRep), 0);
    if (err <= 0)
    {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sockC);
      return -20;
    }

    // Treat validation of own Play Response
    switch (ownPlayRes.err)
    {
    case ERR_OK:
      if (ownPlayRes.validCoup == VALID)
      {
        printf("(Client) coup bien jouee!! \n");
        if (playReq.typeCoup == POS_PION)
        {
          printf("(Client) Ive just made a position movement \n");
        }
        if (ownPlayRes.propCoup == CONT)
        {
          printf("(Client) Game continues! \n");
        }
        else if (ownPlayRes.propCoup == GAGNE)
        {
          printf("(Client)I am winning!\n");
        }
      }
      else
      {
        printf("(Client) Ive made a mistake!! \n");
        return -1;
      }
      break;
    case ERR_COUP:
      printf("tu ne peut pas jouer un coup avant de participer !\n");
      return -1;
      break;
    case ERR_TYP:
      printf("Erreur dans le type de requete\n");
      return -1;
      break;
    default:
      break;
    }
    printf("(client) GOING BACK TO TOP OF LOOP\n");
    printf("\n");
  }

  close(sockC);

  return 0;
}