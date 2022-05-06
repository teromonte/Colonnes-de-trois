#########
#
# Makefile pour miniTCP
#
#########

# For Solaris
#LD_FLAGS = -lsocket -lnsl

# For Linux
LD_FLAGS =

all: Player fonctionsTCP clientTCP serveurTCP

Player: Player.c fonctionsTCP.o javaServerAPI.o
	gcc -Wall Player.c -o Player fonctionsTCP.o javaServerAPI.o $(LD_FLAGS)

fonctionsTCP.o: fonctionsTCP.c fonctionsTCP.h
	gcc -Wall -c fonctionsTCP.c -o fonctionsTCP.o $(LD_FLAGS)

javaServerAPI.o: javaServerAPI.c javaServerAPI.h
	gcc -Wall -c javaServerAPI.c -o javaServerAPI.o $(LD_FLAGS)


clientTCP: clientTCP.c fonctionsTCP
gcc -Wall clientTCP.c -o clientTCP fonctionsTCP.o

serveurTCP: serveurTCP.c fonctionsTCP
gcc -Wall serveurTCP.c -o serveurTCP fonctionsTCP.o colonne-fPIC.o $(LD_FLAGS)


clean:
	rm *~ ; rm -i \#* ; rm *.o; \
        rm Player; rm Player1; rm fonctionsTCP.o; rm javaServerAPI; rm clientTCP ; rm serveurTCP;

