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

		System.out.println("(javaAPI) Server started");

		int portRecv = Integer.parseInt(args[0]);
		ServerSocket srv = new ServerSocket(portRecv);
		Socket[] socks = new Socket[Utils.N_PLAYERS];

		socks[Utils.BLANC] = srv.accept();
		InputStream is0 = socks[Utils.BLANC].getInputStream();
		DataInputStream dis0 = new DataInputStream(is0);
		OutputStream os0 = socks[Utils.BLANC].getOutputStream();
		DataOutputStream dos0 = new DataOutputStream(os0);
		System.out.println("(javaAPI) First player loged in: " +
				socks[Utils.BLANC].getRemoteSocketAddress().toString());

		socks[Utils.NOIR] = srv.accept();
		InputStream is1 = socks[Utils.NOIR].getInputStream();
		DataInputStream dis1 = new DataInputStream(is1);
		OutputStream os1 = socks[Utils.NOIR].getOutputStream();
		DataOutputStream dos1 = new DataOutputStream(os1);
		System.out.println("(javaAPI) Second player loged in: " +
				socks[Utils.NOIR].getRemoteSocketAddress().toString());

		System.out.println("(javaAPI) Listening...");

		Game game = new Game();
		int turn = 0;
		int input = -1;
		Response response;
		while (input != 2) {
			switch (turn) {
				case Utils.BLANC:
					input = Integer.reverseBytes(dis0.readInt());
					response = callAPI(input, game);
					sendResponse(dos0, response);
					break;
				case Utils.NOIR:
					input = Integer.reverseBytes(dis1.readInt());
					response = callAPI(input, game);
					sendResponse(dos1, response);
					break;
			}
			if (turn == 0)
				turn = 1;
			else
				turn = 0;
		}

		srv.close();
		close(socks[Utils.BLANC], dos0, os0, dis0, is0);
		close(socks[Utils.NOIR], dos1, os1, dis1, is1);
		System.out.println("(javaAPI) Server closed!");

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
