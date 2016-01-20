//package sdk_lab.loadgen;

import java.io.*;
import java.net.*;
import java.nio.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Iterator;
//import sdk_lab.loadgen.protocol_message;

public class load_generator {

	private static void send_message_internal (protocol_message message){
  	try{
		Socket sock = new Socket("localhost",4555);
                OutputStream os = sock.getOutputStream();
                String req = message.serialize_message();
                System.out.println("Sending: " + req);
                os.write(req.getBytes(java.nio.charset.Charset.forName("US-ASCII")));
                os.flush();
                os.close();
                sock.close();
	} catch (Exception e) {
		System.out.println(e.toString());
	}
	}

	private static void send_message (protocol_message message){
		send_message_internal(message);
		}

	public static void main(String[] args) {
		send_messages();	
	}

	private static void send_messages() {

	try {
		System.out.println("Generating load");

		protocol_message[] dataToSend;
		dataToSend = new protocol_message[3];
		dataToSend[0] = new protocol_message("HEARTBEAT");
		dataToSend[0].add_field("DATA","nothing");
                dataToSend[1] = new protocol_message("UPDATE");
                dataToSend[1].add_field("DATA","nothing");
                dataToSend[2] = new protocol_message("TEST");
                dataToSend[2].add_field("DATA","nothing");
		
		for (int i=0; i<3; i++) {
			send_message(dataToSend[i]);
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

