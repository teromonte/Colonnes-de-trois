import java.net.ServerSocket;
import java.net.Socket;
import java.io.InputStream;
import java.io.IOException;

public class IAServer {    
    public static void main(String [] args) {
	if (args.length != 1){
	    System.out.println("argument - port");
	    System.exit(1);
	}
	ServerSocket srv ;
	int port = Integer.parseInt(args[0]) ;
	
	try {
	    srv = new ServerSocket(port) ;
        
        while(true) {
            Socket s = srv.accept() ;
            InputStream is = s.getInputStream();
            
            byte[] tablo = new byte[4];
            int recu = is.read(tablo);
        }
	    
	    is.close();
	    s.close();
	    srv.close();
	} catch(IOException e) { }
    }
}
