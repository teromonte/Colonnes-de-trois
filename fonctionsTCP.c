#include "fonctionsTCP.h"

int socketServeur(ushort nPort){
  int sockConx = socket(AF_INET, SOCK_STREAM, 0);
  if (sockConx < 0) {
    perror("(serveurTCP) erreur de socket");
    return -2;
  }
	
	struct sockaddr_in addServ;
	addServ.sin_family = AF_INET;
  addServ.sin_port = htons(nPort); // conversion en format réseau (big endian)
  addServ.sin_addr.s_addr = INADDR_ANY; 
  // INADDR_ANY : 0.0.0.0 (IPv4) donc htonl inutile ici, car pas d'effet
  bzero(addServ.sin_zero, 8);
  
  int sizeAddr = sizeof(struct sockaddr_in);
	
	int err = bind(sockConx, (struct sockaddr *)&addServ, sizeAddr);
  if (err < 0) {
    perror("(serveurTCP) erreur sur le bind");
    close(sockConx);
    return -3;
  }
  
   /* 
   * utilisation en socket de controle, puis attente de demandes de 
   * connexion.
   */
  err = listen(sockConx, 1);
  if (err < 0) {
    perror("(serveurTCP) erreur dans listen");
    close(sockConx);
    return -4;
  }
	return sockConx;
}




int socketClient(char* nomMachine, ushort nPort){
	/* 
   * creation d'une socket, domaine AF_INET, protocole TCP 
   */
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("(client) erreur sur la creation de socket");
    return -2;
  }

  /* 
   * initialisation de l'adresse de la socket - version inet_aton
   */
  /*
  addSockServ.sin_family = AF_INET;
  err = inet_aton(ipMachServ, &addSockServ.sin_addr);
  if (err == 0) { 
    perror("(client) erreur obtention IP serveur");
    close(sock);
    return -3;
  }
  
  addSockServ.sin_port = htons(port);
  bzero(addSockServ.sin_zero, 8);
 	
  sizeAdd = sizeof(struct sockaddr_in);
  */
  
  /* 
   *  initialisation de l'adresse de la socket - version getaddrinfo
   */
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; // AF_INET / AF_INET6 
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  
  
  // récupération de la liste des adresses corespondante au serveur
  struct addrinfo *result;
  char ptr[8];
  sprintf(ptr,"%d",nPort);
  int err = getaddrinfo(nomMachine, ptr, &hints, &result);
  if (err != 0) {
    perror("(client) erreur sur getaddrinfo");
    close(sock);
    return -3;
  }
  
  struct sockaddr_in addSockServ = *(struct sockaddr_in*) result->ai_addr;
  int sizeAdd = result->ai_addrlen;
  
  
  /* 
   * connexion au serveur 
   */
  err = connect(sock, (struct sockaddr *)&addSockServ, sizeAdd); 

  if (err < 0) {
    perror("(client) erreur a la connection de socket");
    close(sock);
    return -4;
  }
  return sock;
}



