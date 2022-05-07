How to run the program:

Colonnes-de-trois/c : make clean
Colonnes-de-trois/c : make

Colonnes-de-trois/c/bin : ./clientTCP 127.0.0.1 55555 6666
Colonnes-de-trois/c/bin : ./clientTCP 127.0.0.2 55555 6666
Colonnes-de-trois/c/bin : ./serveurTCP 55555

Colonnes-de-trois : ~/.vscode/extensions/redhat.java-1.6.0-linux-x64/jre/17.0.2-linux-x86_64/bin/java -XX:+ShowCodeDetailsInExceptionMessages -cp ~/Colonnes-de-trois/ColonnesDeTrois/target/classes server.IAServer 6666