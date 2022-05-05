package server;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

import entities.Request;

public class IAServer {

	public static void main(String[] args) {

		/* verification des arguments */
		if (args.length != 1) {
			System.out.println("usage : portRecv");
			return;
		}
		int portRecv = Integer.parseInt(args[0]);

		try {
			ServerSocket srv = new ServerSocket(portRecv);
			Socket sockComm1 = srv.accept();

			InputStream is1 = sockComm1.getInputStream();
			DataInputStream dis1 = new DataInputStream(is1);
			OutputStream os1 = sockComm1.getOutputStream();
			DataOutputStream dos1 = new DataOutputStream(os1);

			System.out.println("1 aceito");

			Socket sockComm2 = srv.accept();

			InputStream is2 = sockComm2.getInputStream();
			DataInputStream dis2 = new DataInputStream(is2);
			OutputStream os2 = sockComm2.getOutputStream();
			DataOutputStream dos2 = new DataOutputStream(os2);

			System.out.println("2 aceito");

			int i = 0;
			while (i < 10) {
				Request request = new Request();

				request.color = (char) Integer.reverseBytes(dis1.readInt());
				request.type = (char) Integer.reverseBytes(dis1.readInt());
				request.status = (char) Integer.reverseBytes(dis1.readInt());

				System.out.println("Received : " + request.color + " " + request.type + " " + request.status);

				dos1.writeInt(Integer.reverseBytes('a'));

				System.out.println("Sent");

				request.color = (char) Integer.reverseBytes(dis2.readInt());
				request.type = (char) Integer.reverseBytes(dis2.readInt());
				request.status = (char) Integer.reverseBytes(dis2.readInt());

				System.out.println("Received : " + request.color + " " + request.type + " " +
						request.status);

				dos2.writeInt(Integer.reverseBytes('b'));

				System.out.println("Sent");

				i++;
			}

			is1.close();
			os1.close();
			sockComm1.close();

			is2.close();
			os2.close();
			sockComm2.close();
			srv.close();

		} catch (IOException e) {

		}
	}
}
