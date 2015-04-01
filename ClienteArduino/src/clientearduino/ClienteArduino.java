import java.io.*;
import java.net.*;

public class ClienteArduino{
  private String serverIP =  "192.168.0.200";
  private int serverPorta=8888;
    
	public void ClienteAcao (String  msgToServer) throws Exception{
		  String msgFromServer="";
		  Socket clientSocket = new Socket(serverIP, serverPorta);//
		  System.out.println("Conectado no Servidor:"+serverIP+" na Porta:"+serverPorta);
		  DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
		  BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		  outToServer.writeBytes(msgToServer+" \n");
		  System.out.println("Mensagem para Arduino-Server: "+msgToServer);
		  msgFromServer = inFromServer.readLine();
		  System.out.println("Resposta de Arduino-Server: " + msgFromServer);
		  clientSocket.close();
		  
    }
         
}