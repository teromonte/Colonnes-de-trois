package server;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

import entities.Request;
import entities.Response;

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

					Request request = new Request();
					request.color = Integer.reverseBytes(dis1.readInt());
					System.out.println("(javaAPI) Received request with color :" + request.color);

					Response response = new Response();
					response.moveType = 1;
					response.depCol = 2;
					response.depLg = 3;
					response.arrCol = 4;
					response.arrLg = 5;

					dos1.writeInt(Integer.reverseBytes(response.moveType));
					dos1.writeInt(Integer.reverseBytes(response.depCol));
					dos1.writeInt(Integer.reverseBytes(response.depLg));
					dos1.writeInt(Integer.reverseBytes(response.arrCol));
					dos1.writeInt(Integer.reverseBytes(response.arrLg));
					System.out.println("(javaAPI sent resonse. Type of move: " + response.moveType);

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
