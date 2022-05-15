#!/bin/bash

#sh test.sh 127.0.0.1 127.0.0.2 55555 thiago farah 6666 6667

echo "Host 1: $1; Host 2: $2; Port: $3; Player 1: $4; Player 2: $5; IaPort 1: $6; IaPort 2: $7."

# make c code

make clean
make

# Compile and run java Server

#cd ColonnesDeTrois
#mvn clean
#mvn install
#
#gnome-terminal -- mvn exec:java -Dexec.mainClass=server.IAServer -Dexec.args="$6"
#
#gnome-terminal -- mvn exec:java -Dexec.mainClass=server.IAServer -Dexec.args="$7"
#
## run C Server
#
#cd ../

cd c/bin
gnome-terminal -- ./serveurTCP $3

# run both clients

gnome-terminal -- ./clientTCP $1 $3 $6 $4

gnome-terminal -- ./clientTCP $2 $3 $7 $5
