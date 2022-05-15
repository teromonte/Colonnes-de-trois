#!/bin/bash

#echo "Host: $1; Port: $2; Player: $3; IaPort: $4."

# make c code

make clean
make

# Compile and run java Server

cd ColonnesDeTrois

mvn compile
mvn clean install

gnome-terminal -- mvn exec:java -Dexec.mainClass=server.IAServer -Dexec.args="$4"

# run client

cd ../c/bin

gnome-terminal -- ./clientTCP $1 $2 $4 
# run this command inside c program && $3


