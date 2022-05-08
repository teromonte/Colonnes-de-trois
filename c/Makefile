#########
#
# Makefile pour miniTCP
#
#########

# For Linux
LD_FLAGS = -Wall

all: fonctionsTCP protocolJava functionsClient functionsServer clientTCP serveurTCP

fonctionsTCP: lib/src/fonctionsTCP.c lib/headers/fonctionsTCP.h
	gcc -c lib/src/fonctionsTCP.c -o bin/fonctionsTCP.o $(LD_FLAGS)

protocolJava: lib/src/protocolJava.c lib/headers/protocolJava.h
	gcc -c lib/src/protocolJava.c -o bin/protocolJava.o $(LD_FLAGS)

functionsClient: lib/src/functionsClient.c lib/headers/functionsClient.h
	gcc -c lib/src/functionsClient.c -o bin/functionsClient.o $(LD_FLAGS)

functionsServer: lib/src/functionsServer.c lib/headers/functionsServer.h
	gcc -c lib/src/functionsServer.c -o bin/functionsServer.o $(LD_FLAGS)

clientTCP: src/clientTCP.c fonctionsTCP
	gcc src/clientTCP.c -o bin/clientTCP bin/fonctionsTCP.o bin/protocolJava.o bin/functionsClient.o $(LD_FLAGS)

serveurTCP: src/serveurTCP.c fonctionsTCP
	gcc src/serveurTCP.c -o bin/serveurTCP bin/fonctionsTCP.o bin/colonne-fPIC.o bin/functionsServer.o $(LD_FLAGS)

clean:
	rm bin/fonctionsTCP.o; rm bin/protocolJava.o; rm bin/functionsClient.o; rm bin/functionsServer.o; rm bin/clientTCP ; rm bin/serveurTCP;
