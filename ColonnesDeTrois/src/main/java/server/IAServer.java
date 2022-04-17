package server;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class IAServer {

	public static void main(String[] args) {

		float resultat = 0;
		int portRecv;
		Op op = new Op();

		/* verification des arguments */
		if (args.length != 1) {
			System.out.println("usage : portRecv");
			return;
		}
		System.out.println(args[0]);
		portRecv = Integer.parseInt(args[0]);

		ServerSocket srv;
		try {
			srv = new ServerSocket(portRecv);
			Socket sockComm = srv.accept();

			InputStream is = sockComm.getInputStream();
			DataInputStream dis = new DataInputStream(is);
			OutputStream os = sockComm.getOutputStream();
			DataOutputStream dos = new DataOutputStream(os);

			int err = 0;
			while (err >= 0) {

				op.opt = (char) Integer.reverseBytes(dis.readInt());
				op.opd1 = reverseFloat(dis.readFloat());
				op.opd2 = reverseFloat(dis.readFloat());
				System.out.println("Received : " + op.opt + " " + op.opd1 + " " + op.opd2);

				resultat = op.operation();

				System.out.println("Result :" + resultat);
				dos.writeFloat(reverseFloat(resultat));
				System.out.println("Result reverse :" + reverseFloat(resultat));

			}
			is.close(); // fermer le flux
			os.close();
			sockComm.close();
			srv.close(); // fermer la socket de comm et de connex
		} catch (IOException e) {
			// TODO
		}
	}

	public static byte[] floatToByteArray(float value) {
		int intBits = Float.floatToIntBits(value);
		return new byte[] {
				(byte) (intBits >> 24), (byte) (intBits >> 16), (byte) (intBits >> 8), (byte) (intBits) };
	}

	public static float byteArrayToFloat(byte[] bytes) {
		int intBits = bytes[0] << 24 | (bytes[1] & 0xFF) << 16 | (bytes[2] & 0xFF) << 8 | (bytes[3] & 0xFF);
		return Float.intBitsToFloat(intBits);
	}

	public static byte[] reverse(byte[] bytes) {

		byte[] retVal = new byte[4];
		// swap the bytes into a temporary buffer
		retVal[0] = bytes[3];
		retVal[1] = bytes[2];
		retVal[2] = bytes[1];
		retVal[3] = bytes[0];
		return retVal;
	}

	public static float reverseFloat(float f) {
		return byteArrayToFloat(reverse(floatToByteArray(f)));
	}
}
