#include "../lib/headers/protocolJava.h"
#include "../header/protocolColonne.h"
#include "../lib/headers/fonctionsTCP.h"

int main(int argc, char **argv)
{

  int sockC,
      portC,  /* variables de lecture */
      sockAI, /* descripteur de la socket locale */
      portAI,
      err;

  char chaine[30], dem, *nomMachServ;

  TPartieReq req;            // Participation request
  TPartieRep rep;            // Answer to the participation request
  TCase square;              // Case
  TCoupReq playRequest;      // Play request
  TCoupRep ownResponse;      // Response to the play request
  TCoupRep opponentResponse; // delete

  // verification des arguments
  if (argc != 4)
  {
    printf("usage : %s nom/IPServ portC portAI\n", argv[0]);
    return -1;
  }

  nomMachServ = argv[1];
  portC = atoi(argv[2]);
  portAI = atoi(argv[3]);

  /////////////////////////////////////// CONNECT TO C SERVER ///////////////////////////////////////////////////////////////////////

  sockC = socketClient(nomMachServ, portC);

  /////////////////////////////////////// CONNECT TO IA SERVER ///////////////////////////////////////////////////////////////////////

  sockAI = socketClient(nomMachServ, portAI);

  /////////////////////////////////////// ASK FOR PARTICIPATION ///////////////////////////////////////////////////////////////////////

  printf("(client) tapez p pour participer ainsi votre nom ou login : ");
  scanf("%c %s", &dem, chaine);
  printf("(client) envoi de - %s - \n", chaine);

  switch (dem)
  {
  case 'p':
    req.idRequest = PARTIE;
    break;
  default:
    break;
  }

  // Requesting to play
  strncpy(req.nomJoueur, chaine, sizeof(chaine));
  printf("(client) send nom est %s \n", req.nomJoueur);
  err = send(sockC, &req, sizeof(TPartieReq), 0);
  if (err <= 0)
  {
    perror("(client) erreur sur le send");
    shutdown(sockC, SHUT_RDWR);
    close(sockC);
  }

  // Receive anwser from server
  err = recv(sockC, &rep, sizeof(TPartieRep), 0);
  if (err <= 0)
  {
    perror("(Client) erreur dans la reception");
    close(sockC);
    return -6;
  }

  switch (rep.err)
  {
  case ERR_OK:
    if (rep.coul == BLANC)
    {
      printf("(Client) vous etes le jouer blanc et le nom de votre adverse c'est : %s\n", rep.nomAdvers);
      break;
    }
    if (rep.coul == NOIR)
    {
      printf("(Client) vous etes le jouer noir et le nom de votre adverse c'est : %s\n", rep.nomAdvers);
      break;
    }
  case ERR_PARTIE:
    printf("pas possible de participer !\n");
    break;
  default:
    break;
  }
  printf("\n");

  /////////////////////////////////////// FINISH OF ASKING PARTICIPATION //////////////////////////////////////////////////////////////

  if (rep.coul == BLANC)
  {
    printf("(Client) you are the first player! lancer un coup\n");
    // Arrange Play protocol package
    // TODO ask the IA for a coup and get the result
    int color = 0;
    int responseAI;
    responseAI = requestAI(color, sockAI);
    printf("WOW::::: %c\n", responseAI);
    
    playRequest.coul = rep.coul;
    playRequest.idRequest = COUP;
    playRequest.typeCoup = POS_PION;
    square.lg = DEUX;
    square.col = C;
    playRequest.action.posPion = square;
    // Send play
    err = send(sockC, &playRequest, sizeof(TCoupReq), 0);
    if (err <= 0)
    {
      perror("(client) erreur sur le send de coup ");
      shutdown(sockC, SHUT_RDWR);
      close(sockC);
    }
    // Receive validation of own play
    err = recv(sockC, &ownResponse, sizeof(TCoupRep), 0);
    if (err <= 0)
    {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sockC);
      return -20;
    }
    printf("(cclient) BLANC send play - first play!!!\n");
    printf("\n");
    // Treat Play Response
    switch (ownResponse.err)
    {
    case ERR_OK:
      if (ownResponse.validCoup == VALID)
      {
        printf("(Client) coup bien jouee!! \n");
        if (ownResponse.propCoup == CONT)
        {
          printf("(Client) votre coup est valid et aucun gagne pour le moment! \n");
        }
        else if (ownResponse.propCoup == GAGNE)
        {
          printf("(Client) you are winning! \n");
        }
      }
      else
      {
        // TODO treat TIMEOUT AND TRICHE
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
    err = recv(sockC, &opponentResponse, sizeof(TCoupRep), 0);
    if (err <= 0)
    {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sockC);
      return -20;
    }
    printf("(client) Received validation of adversaire play \n");

    // Treat adversaire play
    switch (opponentResponse.err)
    {
    case ERR_OK:
      if (opponentResponse.validCoup == VALID)
      {
        printf("(Client) coup adverse bien jouee!! \n");
        if (playRequest.typeCoup == POS_PION)
        {
          printf("(Client) il a fait un pos \n");
        }
        else if (opponentResponse.propCoup == CONT)
        {
          printf("(Client) aucun gagne pour le moment! \n");
        }
        else if (opponentResponse.propCoup == GAGNE)
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
    playRequest.idRequest = COUP;
    playRequest.typeCoup = POS_PION;
    playRequest.coul = rep.coul;
    square.lg = TROIS;
    square.col = B;
    playRequest.action.posPion = square;
    // Send play
    err = send(sockC, &playRequest, sizeof(TCoupReq), 0);
    if (err <= 0)
    {
      perror("(client) erreur sur le send de coup ");
      shutdown(sockC, SHUT_RDWR);
      close(sockC);
    }
    if (playRequest.coul == BLANC)
      printf("(client) BLANC send play\n");
    else
      printf("(client) NOIR send play \n");

    // Receive validation of own play
    err = recv(sockC, &ownResponse, sizeof(TCoupRep), 0);
    if (err <= 0)
    {
      perror("(Client) erreur dans la reception de resultats coup ");
      close(sockC);
      return -20;
    }

    // Treat validation of own Play Response
    switch (ownResponse.err)
    {
    case ERR_OK:
      if (ownResponse.validCoup == VALID)
      {
        printf("(Client) coup bien jouee!! \n");
        if (playRequest.typeCoup == POS_PION)
        {
          printf("(Client) Ive just made a position movement \n");
        }
        if (ownResponse.propCoup == CONT)
        {
          printf("(Client) Game continues! \n");
        }
        else if (ownResponse.propCoup == GAGNE)
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
