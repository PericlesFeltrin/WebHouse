#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address, IP address and Portnumber for your Server below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress serverIP(192,168,0,200);
int serverPort=8888;
 
// Initialize the Ethernet server library
// with the IP address and port you want to use
EthernetServer server(serverPort);

///Sensor ultra som
int trig=48;    //configura o pino 8 da Arduino para conexão do Trig do sonar
int echo=42;    //configura o pino 7 da Arduino para conexão do Echo do sonar
int statusAlarme=0;
int trava=30;
int alarme=21;  //Sirene
int led=37;
String status_alarme="false";
long duration;
long cm; 

void setup(){
  // start the serial for debugging
  Serial.begin(9600);
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, serverIP);
  server.begin();
  Serial.println("Server started");//log
  
  //Saidas da Casa
  pinMode(trava, OUTPUT);
  pinMode(alarme, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);   //define o pino 8 como um output (envia sinal do sensor)
  pinMode(echo, INPUT);    //define o pino 7 como um input (recebe sinal do sensor)

}


void loop(){
  delay(100);
  digitalWrite(trig, LOW); 
  delayMicroseconds(2);  
  digitalWrite(trig, HIGH); //ativa o sonar por 10 microssegundos
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  delayMicroseconds(2);
  duration = pulseIn(echo, HIGH); //duration recebe o tempo de ida e volta do sinal
  cm = duration/58; //converte duration em centímetros
  if(statusAlarme==1){
     Serial.println("Client nao existe E ");//print it to the serial
     //comandos para enviar um pulso do sonar e armazenar o tempo de respos
     Serial.println(statusAlarme);
     delay(10);
     if(cm<50){
         digitalWrite(alarme, HIGH);
      }else{
         digitalWrite(alarme, LOW);
      }
      
   }
   Serial.println(cm);
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    String clientMsg ="";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        clientMsg+=c;//store the recieved chracters in a string
        //if the character is an "end of line" the whole message is recieved
        if (c == '\t') {// Condição Busca
          Serial.println("Client: "+clientMsg);//print it to the serial
          if(clientMsg=="322 \t"){
            client.println("Seja Bem Vindo");
            digitalWrite(trava, HIGH);
            delay(10);
            digitalWrite(trava, LOW);
          }else if(clientMsg=="STATUS \t"){
            client.println(status_alarme);  
          }else{
             client.println("Erro");//modify the string and send it bac
          }
          clientMsg="";
          //Fim Condição Busca
        }else if (c == '\n') { // Condição Ação
          Serial.println("Client: "+clientMsg);//print it to the serial
          
        if(clientMsg=="L A \n"){
            statusAlarme=1;
            client.println("Alarme Ligada");
            digitalWrite(alarme, HIGH);
            digitalWrite(led, HIGH);
            status_alarme="true"; 
        }else if(clientMsg=="D A \n"){
            statusAlarme=0;
            client.println("Alarme Desligada");
            digitalWrite(alarme, LOW);
            digitalWrite(led, LOW);
            status_alarme="false";
        }else{
             client.println("Erro");//modify the string and send it bac
        }
                  
          clientMsg="";
        } // Fim condição Ação
        
        
      }
    }
    //Serial.println("Client nao existe ");//print it to the serial
    // give the Client time to receive the data
    //delay(10);
    // close the connection:
    client.stop();
  }
}
