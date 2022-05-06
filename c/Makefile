#########
#
# Makefile pour miniTCP
#
#########

# For Linux
LD_FLAGS =

all: fonctionsTCP clientTCP serveurTCP

fonctionsTCP: lib/src/fonctionsTCP.c lib/headers/fonctionsTCP.h
	gcc -Wall -c lib/src/fonctionsTCP.c -o bin/fonctionsTCP.o $(LD_FLAGS)

clientTCP: src/clientTCP.c fonctionsTCP
	gcc -Wall src/clientTCP.c -o bin/clientTCP bin/fonctionsTCP.o $(LD_FLAGS)

serveurTCP: src/serveurTCP.c fonctionsTCP
	gcc -Wall src/serveurTCP.c -o bin/serveurTCP bin/fonctionsTCP.o bin/colonne-fPIC.o $(LD_FLAGS)

clean:
	rm bin/fonctionsTCP.o; rm bin/clientTCP ; rm bin/serveurTCP;
