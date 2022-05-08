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

		Game game = new Game();

		while (true) {
			System.out.println("(javaAPI) Server started");

			try (ServerSocket srv = new ServerSocket(portRecv)) {
				while (true) {

					Socket sock = srv.accept();
					System.out.println("(javaAPI) Accept: " + sock.getRemoteSocketAddress().toString());

					InputStream is = sock.getInputStream();
					DataInputStream dis = new DataInputStream(is);
					OutputStream os = sock.getOutputStream();
					DataOutputStream dos = new DataOutputStream(os);

					int input = Integer.reverseBytes(dis.readInt());

					Response response = callAPI(input, game);

					sendResponse(dos, response);

					close(sock, dos, os, dis, is);
				}
			} catch (Exception e) {
				System.out.println("Exception!");
			}
		}

	}

	private static Response callAPI(int input, Game game) {
		switch (input) {
			case 0:
				System.out.println("(javaAPI) Processing BLANC request");

				return game.getNextMove(Utils.BLANC);
			case 1:
				System.out.println("(javaAPI) Processing NOIR request");

				return game.getNextMove(Utils.NOIR);
			default:
				System.out.println("(javaAPI) Reset match");
				return game.reset();
		}
	}

	private static void sendResponse(DataOutputStream dos, Response response) throws IOException {
		switch (response.moveType) {

			case Utils.PLACE:
				System.out.println("(javaAPI) Sent resonse. PLACE");

				break;
			case Utils.MOVE:
				System.out.println("(javaAPI) Sent resonse. MOVE");

				break;
			case Utils.RESET:
				System.out.println("(javaAPI) Going to the second match");
				break;
		}
		dos.writeInt(Integer.reverseBytes(response.moveType));
		dos.writeInt(Integer.reverseBytes(response.depCol));
		dos.writeInt(Integer.reverseBytes(response.depLg));
		dos.writeInt(Integer.reverseBytes(response.arrCol));
		dos.writeInt(Integer.reverseBytes(response.arrLg));
	}

	private static void close(Socket sock, DataOutputStream dos, OutputStream os, DataInputStream dis, InputStream is)
			throws IOException {
		is.close();
		os.close();
		dis.close();
		dos.close();
		sock.close();
	}
}
