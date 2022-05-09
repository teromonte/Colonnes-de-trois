/*
 *
 * Programme : protocolColonne.h
 *
 * Synopsis : entete du protocole d'acces a l'arbitre
 *            pour le jeu Colonnes de trois
 *
 * Ecrit par : VF, FB
 * Date :  07 / 03 / 22
 *
 */

#ifndef PROTOCOLCOLONNE_H
#define PROTOCOLCOLONNE_H

#include <stdio.h>

/* Taille des chaines de caracteres pour les noms */
#define TNOM 30
#define NUM_OF_MATCHES 2

/* Identificateurs des requetes */
enum TIdRequest
{
  PARTIE = 0,
  COUP = 1
};

/* Types d'erreur */
enum TErreur
{
  ERR_OK = 0,     /* Validation de la requete */
  ERR_PARTIE = 1, /* Erreur sur la demande de partie */
  ERR_COUP = 2,   /* Erreur sur le coup joue */
  ERR_TYP = 3     /* Erreur sur le type de requete */
};

/*
 * Structures demande de partie
 */
struct TPartieReq
{
  enum TIdRequest idRequest; /* Identificateur de la requete */
  char nomJoueur[TNOM];      /* Nom du joueur */
};

enum TPion
{
  BLANC = 0,
  NOIR = 1
};

struct TPartieRep
{
  enum TErreur err;     /* Code d'erreur */
  enum TPion coul;      /* Couleur du pion */
  char nomAdvers[TNOM]; /* Nom du joueur */
};

/*
 * Definition d'une position de case
 */
enum TLigne
{
  UN = 0,
  DEUX = 1,
  TROIS = 2
};
enum TCol
{
  A = 0,
  B = 1,
  C = 2
};

struct TCase
{
  enum TLigne lg; /* La ligne de la position d'une case */
  enum TCol col;  /* La colonne de la position d'une case */
};

/*
 * Definition d'un deplacement de pion
 */
struct TDeplPion
{
  struct TCase caseDep; /* Position de depart du pion */
  struct TCase caseArr; /* Position d'arrivee du pion */
};

/*
 * Structures coup du joueur
 */

/* Precision des types de coups */
enum TCoup
{
  POS_PION = 0,
  DEPL_PION = 1,
  PASSE = 2
};

struct TCoupReq
{
  enum TIdRequest idRequest; /* Identificateur de la requete */
  enum TCoup typeCoup;       /* Type du coup : positionnement ou deplacement */
  enum TPion coul;           /* Couleur de pion */
  union
  {
    struct TCase posPion;      /* Positionnement de pion */
    struct TDeplPion deplPion; /* Deplacement de pion */
  } action;
};

/* Validite du coup */
enum TValCoup
{
  VALID = 0,   // cont gagne
  TIMEOUT = 1, // nulle
  TRICHE = 2   // perdu
};

/* Propriete des coups */
enum TPropCoup
{
  CONT = 0,
  GAGNE = 1,
  NULLE = 2,
  PERDU = 3
};

/* Reponse a un coup */
struct TCoupRep
{
  enum TErreur err;        /* Code d'erreur */
  enum TValCoup validCoup; /* Validite du coup */
  enum TPropCoup propCoup; /* Propriete du coup */
};

#endif
