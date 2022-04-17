#define ERR_CALC -1
#define OK_CALC 0

struct request
{
    char color;
    char type;
    char status;
};

struct pair
{
    char x;
    char y;
};

/* LIRE OPERATION BINAIRE ARITHMETIQUE
+ / - / * / /
ignore si operateur invalide
*/
void lireOperation(char *operateur, float *operande1, float *operande2);
