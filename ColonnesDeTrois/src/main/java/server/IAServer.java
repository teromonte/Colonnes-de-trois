package server;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

import entities.Request;

public class IAServer {

	public static void main(String[] args) throws IOException {

		/* verification des arguments */
		if (args.length != 1) {
			System.out.println("usage : portRecv");
			return;
		}

		int portRecv = Integer.parseInt(args[0]);

		while (true) {

			try (ServerSocket srv = new ServerSocket(portRecv)) {
				while (true) {
					Socket sockComm1 = srv.accept();
					System.out.println("Accept: " + sockComm1.getRemoteSocketAddress().toString());

					InputStream is1 = sockComm1.getInputStream();
					DataInputStream dis1 = new DataInputStream(is1);
					OutputStream os1 = sockComm1.getOutputStream();
					DataOutputStream dos1 = new DataOutputStream(os1);

					Request request1 = new Request();

					request1.color = (char) Integer.reverseBytes(dis1.readInt());
					request1.type = (char) Integer.reverseBytes(dis1.readInt());
					request1.status = (char) Integer.reverseBytes(dis1.readInt());

					System.out.println("Received : " + request1.color + " " +
							request1.type + " " + request1.status);

					dos1.writeInt(Integer.reverseBytes('a'));

					System.out.println("Sent a");

					is1.close();
					os1.close();
					dis1.close();
					dos1.close();
					sockComm1.close();
				}
			} catch (Exception e) {
				System.out.println("Exception!");
			}
		}

	}
}
