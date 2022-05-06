#########
#
# Makefile pour miniTCP
#
#########

# For Linux
LD_FLAGS = -Wall

all: fonctionsTCP protocolJava clientTCP serveurTCP

fonctionsTCP: lib/src/fonctionsTCP.c lib/headers/fonctionsTCP.h
	gcc -c lib/src/fonctionsTCP.c -o bin/fonctionsTCP.o $(LD_FLAGS)

protocolJava: lib/src/protocolJava.c lib/headers/protocolJava.h
	gcc -c lib/src/protocolJava.c -o bin/protocolJava.o $(LD_FLAGS)

clientTCP: src/clientTCP.c fonctionsTCP
	gcc src/clientTCP.c -o bin/clientTCP bin/fonctionsTCP.o bin/protocolJava.o $(LD_FLAGS)

serveurTCP: src/serveurTCP.c fonctionsTCP
	gcc src/serveurTCP.c -o bin/serveurTCP bin/fonctionsTCP.o bin/colonne-fPIC.o $(LD_FLAGS)

clean:
	rm bin/fonctionsTCP.o; rm bin/protocolJava.o; rm bin/clientTCP ; rm bin/serveurTCP;
