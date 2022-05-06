#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

// Pour socketServeur
extern int err;
extern int sockConx;
extern struct sockaddr_in addServ;
extern int sizeAddr;
// Pour socketClient
extern int sockServerC;
extern struct sockaddr_in addSockServ;
extern int sizeAdd;
extern struct addrinfo hints;
extern struct addrinfo *result;

// Condensé des fonctions socket, bind et listen pour lancer un socket
int socketServeur(ushort nPort);

// Condensé des fonctions socket et connect
int socketClient(char *nomMachine, ushort nPort);
