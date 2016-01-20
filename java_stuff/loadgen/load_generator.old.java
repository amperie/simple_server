import java.io.*;
import java.net.*;
import java.nio.*;

public class load_generator {

	public static void main(String[] args) {

	try {
		System.out.println("Generating load");

		String[] dataToSend;
		dataToSend = new String[3];
		dataToSend[0] = "TYPE=HEARTBEAT|DATA=NULL|";
		dataToSend[1] = "TYPE=UPDATE|DATA=nothing|";
		dataToSend[2] = "TYPE=TEST|DATA=nothing|";
		
		for (int i=0; i<3; i++) {
			Socket sock = new Socket("localhost",4555);
			OutputStream os = sock.getOutputStream();
			String req = dataToSend[i];
			System.out.println("Sending: " + req);
			os.write(req.getBytes(java.nio.charset.Charset.forName("US-ASCII")));
			os.flush();
			os.close();
			sock.close();
			if (i==2) {i=-1;}
			Thread.sleep(2000);
		}
	}
	catch (Exception e)
	{
		System.out.println(e.toString());
	}
	}
}
