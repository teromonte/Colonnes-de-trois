How to run the program:

java -jar target/ColonnesDeTrois-1.0-SNAPSHOT-jar-with-dependencies.jar 6666

sh test.sh 127.0.0.1 127.0.0.2 55555 thiago farah 6666 6667

Colonnes-de-trois/c : make clean

Colonnes-de-trois/c : make

Colonnes-de-trois/c/bin : ./clientTCP 127.0.0.1 55555 6666

Colonnes-de-trois/c/bin : ./clientTCP 127.0.0.2 55555 6667

Colonnes-de-trois/c/bin : ./serveurTCP 55555

Colonnes-de-trois : /usr/bin/env /home/thiago/.vscode/extensions/redhat.java-1.6.0-linux-x64/jre/17.0.2-linux-x86_64/bin/java -XX:+ShowCodeDetailsInExceptionMessages -cp /home/thiago/Desktop/Tournoi/Colonnes-de-trois/ColonnesDeTrois/target/classes server.IAServer 6666

Colonnes-de-trois : /usr/bin/env /home/thiago/.vscode/extensions/redhat.java-1.6.0-linux-x64/jre/17.0.2-linux-x86_64/bin/java -XX:+ShowCodeDetailsInExceptionMessages -cp /home/thiago/Desktop/Tournoi/Colonnes-de-trois/ColonnesDeTrois/target/classes server.IAServer 6667

DEBUG JAVA: /usr/bin/env /home/thiago/.vscode/extensions/redhat.java-1.6.0-linux-x64/jre/17.0.2-linux-x86_64/bin/java -agentlib:jdwp=transport=dt_socket,server=n,suspend=y,address=localhost:40101 -XX:+ShowCodeDetailsInExceptionMessages -cp /home/thiago/Desktop/Tournoi/Colonnes-de-trois/ColonnesDeTrois/target/classes server.IAServer

gnome-terminal -- mvn exec:java -Dexec.mainClass=server.IAServer -Dexec.args="6666"

gnome-terminal -- mvn exec:java -Dexec.mainClass=server.IAServer -Dexec.args="6667"
