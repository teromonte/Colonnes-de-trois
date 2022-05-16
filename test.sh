#!/bin/bash

#sh test.sh 127.0.0.1 127.0.0.2 55555 thiago farah 6666 6667

echo "Host 1: $1; Host 2: $2; Port: $3; Player 1: $4; Player 2: $5; IaPort 1: $6; IaPort 2: $7."

# make c code

make clean
make

# Compile and run java Server

#cd ColonnesDeTrois
#mvn clean install
#cd target
#
#gnome-terminal -- java -jar ColonnesDeTrois-0.1.0-SNAPSHOT-jar-with-dependencies.jar $6
#
#sleep 2
#
#gnome-terminal -- java -jar ColonnesDeTrois-0.1.0-SNAPSHOT-jar-with-dependencies.jar $7
#
#sleep 2
#
#
## run C Server
#
#cd ../../

cd c/bin
gnome-terminal -- ./serveurTCP $3

sleep 2


# run both clients

gnome-terminal -- ./clientTCP $1 $3 $6 $4

sleep 2


gnome-terminal -- ./clientTCP $2 $3 $7 $5






















