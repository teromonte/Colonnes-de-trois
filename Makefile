LD_FLAGS = -Wall

all: fonctionsTCP protocolJava functionsClient functionsServer clientTCP serveurTCP

fonctionsTCP: c/lib/src/fonctionsTCP.c c/lib/headers/fonctionsTCP.h
	gcc -c c/lib/src/fonctionsTCP.c -o c/bin/fonctionsTCP.o $(LD_FLAGS)

protocolJava: c/lib/src/protocolJava.c c/lib/headers/protocolJava.h
	gcc -c c/lib/src/protocolJava.c -o c/bin/protocolJava.o $(LD_FLAGS)

functionsClient: c/lib/src/functionsClient.c c/lib/headers/functionsClient.h
	gcc -c c/lib/src/functionsClient.c -o c/bin/functionsClient.o $(LD_FLAGS)

functionsServer: c/lib/src/functionsServer.c c/lib/headers/functionsServer.h
	gcc -c c/lib/src/functionsServer.c -o c/bin/functionsServer.o $(LD_FLAGS)

clientTCP: c/src/clientTCP.c fonctionsTCP
	gcc c/src/clientTCP.c -o c/bin/clientTCP c/bin/fonctionsTCP.o c/bin/protocolJava.o c/bin/functionsClient.o $(LD_FLAGS)

serveurTCP: c/src/serveurTCP.c fonctionsTCP
	gcc c/src/serveurTCP.c -o c/bin/serveurTCP c/bin/fonctionsTCP.o c/bin/colonne-fPIC.o c/bin/functionsServer.o $(LD_FLAGS)

clean:
	rm c/bin/fonctionsTCP.o; rm c/bin/protocolJava.o; rm c/bin/functionsClient.o; rm c/bin/functionsServer.o; rm c/bin/clientTCP ; rm c/bin/serveurTCP;
