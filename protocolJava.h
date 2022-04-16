#ifndef _protocolJava_h
#define _protocolJava_h

/*
 * Structures demande de partie
 */
typedef struct
{
  int color; /* Identificateur de la requete */
  char column; /* Nom du joueur */
  int row; /* Nom du joueur */
} TPartieReq;