package server;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

import entities.Response;
import game.Game;
import utils.Utils;

public class IAServer {

	public static void main(String[] args) throws IOException {

		/* verification des arguments */
		if (args.length != 1) {
			System.out.println("usage : portRecv");
			return;
		}

		int portRecv = Integer.parseInt(args[0]);
		ServerSocket srv = new ServerSocket(portRecv);

		System.out.println("(javaAPI) Server turned ON. Waiting start request...");

		Socket sock = srv.accept();
		InputStream is = sock.getInputStream();
		DataInputStream dis = new DataInputStream(is);
		OutputStream os = sock.getOutputStream();
		DataOutputStream dos = new DataOutputStream(os);

		Response response;
		Game game = new Game();
		int input, matchNum = 0;

		int startingColor = Integer.reverseBytes(dis.readInt());

		if (startingColor == Utils.BLANC)
			System.out.println("(javaAPI) BLANC started the Server!");
		else
			System.out.println("(javaAPI) NOIR started the Server!");

		System.out.println("(javaAPI) Listening...");

		while (matchNum != Utils.N_GAMES) {
			input = Integer.reverseBytes(dis.readInt());

			if (input != Utils.SET) {
				if (startingColor == Utils.BLANC) {
					if (matchNum == Utils.FIRST_MATCH) {
						System.out.println("(javaAPI) Processing BLANC request");
						response = game.getNextMove(input);
						sendResponse(dos, response);
					} else {
						System.out.println("(javaAPI) Processing NOIR request");
						game.getNextMove(input);
					}
				} else {
					if (matchNum == Utils.FIRST_MATCH) {
						System.out.println("(javaAPI) Processing NOIR request");
						game.getNextMove(input);
					} else {
						System.out.println("(javaAPI) Processing BLANC request");
						response = game.getNextMove(input);
						sendResponse(dos, response);
					}
				}
			} else {
				System.out.println("(javaAPI) Reset match");
				game.reset();
				matchNum++;
			}
		}

		close(sock, dos, os, dis, is, srv);
		System.out.println("(javaAPI) Server closed!");

	}

	private static void sendResponse(DataOutputStream dos, Response response)
			throws IOException {
		if (response == null) {
			return;
		}
		switch (response.moveType) {
			case Utils.PLACE:
				System.out.println("(javaAPI) Sent response. PLACE");
				break;
			case Utils.MOVE:
				System.out.println("(javaAPI) Sent response. MOVE");
				break;
			case Utils.SET:
				System.out.println("(javaAPI) Initialise AI!");
				break;
		}
		dos.writeInt(Integer.reverseBytes(response.moveType));
		dos.writeInt(Integer.reverseBytes(response.depCol));
		dos.writeInt(Integer.reverseBytes(response.depLg));
		dos.writeInt(Integer.reverseBytes(response.arrCol));
		dos.writeInt(Integer.reverseBytes(response.arrLg));
	}

	private static void close(Socket sock, DataOutputStream dos, OutputStream os, DataInputStream dis, InputStream is,
			ServerSocket srv) throws IOException {
		is.close();
		os.close();
		dis.close();
		dos.close();
		sock.close();
		srv.close();

	}
}
