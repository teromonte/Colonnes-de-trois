#########
#
# Makefile pour miniTCP
#
#########

# For Solaris
#LD_FLAGS = -lsocket -lnsl

# For Linux
LD_FLAGS =

all: fonctionsTCP clientTCP serveurTCP

fonctionsTCP: fonctionsTCP.c fonctionsTCP.h
	gcc -Wall -c fonctionsTCP.c -o fonctionsTCP.o $(LD_FLAGS)

clientTCP: clientTCP.c fonctionsTCP
	gcc -Wall clientTCP.c -o clientTCP fonctionsTCP.o

serveurTCP: serveurTCP.c fonctionsTCP
	gcc -Wall serveurTCP.c -o serveurTCP fonctionsTCP.o colonne-fPIC.o $(LD_FLAGS)

clean:
	rm fonctionsTCP.o; rm clientTCP ; rm serveurTCP;

