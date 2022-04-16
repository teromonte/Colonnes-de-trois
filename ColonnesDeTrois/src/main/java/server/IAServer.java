package server;

import java.net.ServerSocket;
import java.net.Socket;

import game.Game;
import utils.Move;
import utils.Pair;
import utils.Utils;

import java.io.InputStream;
import java.io.IOException;

public class IAServer {
	public static void main(String[] args) {
		if (args.length != 1) {
			System.out.println("argument - port");
			System.exit(1);
		}
		ServerSocket srv;
		int port = Integer.parseInt(args[0]);

		try {
			srv = new ServerSocket(port);

			Socket s = srv.accept();
			InputStream is = s.getInputStream();

			byte[] tablo = new byte[4];
			int recu = is.read(tablo);

			Game g = new Game();
			Pair p = g.place(Utils.Color.Black);
			Move m = g.move(Utils.Color.White);

			is.close();
			s.close();
			srv.close();
		} catch (IOException e) {
		}
	}
}
