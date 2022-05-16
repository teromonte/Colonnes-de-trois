#!/bin/bash

#echo "Host: $1; Port: $2; Player: $3; IaPort: $4."

# make c code

make clean
make

# Compile and run java Server

cd ColonnesDeTrois
mvn clean install

cd target
gnome-terminal -- java -jar ColonnesDeTrois-0.1.0-SNAPSHOT-jar-with-dependencies.jar $4

# run client

cd ../../
cd c/bin

gnome-terminal -- ./clientTCP $1 $2 $4 $3
