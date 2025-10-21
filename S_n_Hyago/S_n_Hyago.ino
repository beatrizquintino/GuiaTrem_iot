#include <WiFi.h>
#include <PubSubClient.h>
#include <env.h>

WiFiClient client;          //cria objeto p/ WiFi
PubSubClient mqtt(client);  //cria objeto p/ mqtt usando WiFi

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;

const String brokerUser = "";  //variável para o user do brocker
const String brokerPass = "";  //variável para a senha do brocker



const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(115200);    /#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;          //cria objeto p/ WiFi
PubSubClient mqtt(client);  //cria objeto p/ mqtt usando WiFi

const byte ledPin = 2;

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topico = "topicoHY"; 

const String brokerUser = "";  //variável para o user do brocker
const String brokerPass = "";  //variável para a senha do brocker



const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);    //configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS);  // tenta conectar na rede
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("\nConectado com sucesso!");



  //Configura a placa para mostra na tela
  mqtt.setServer(BROKER_URL, BROKER_PORT);
  Serial.println("Conectando no Broker");
  
  String boardID = "S1-"; //Cria um nome que começa com "s1-"
  boardID += String(random(0xffff),HEX); //Junta o "s1-" com um número aleatório Hexadecimal

  //Enquanto não estiver conectado mostra "."
  
  while (!mqtt.connected()){
    mqtt.connect(userId.c_str(), BROKER_USR_NAME, BROKER_USR_PASS);
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(TOPIC1); //vamos mudar (topico)
  mqtt.setCallback(callback);
  Serial.println("\nConectado com sucesso ao broker");
}

void loop() {
  /*String msg = "TESTE"; //Informação que será enviada para o broker
  String topico = "AulaIoT/msg";
  mqtt.publish(topico.c_str(), msg.c_str());
  delay(1000);
  Serial.println(msg);
  mqtt.loop();*/
 

 //envia a mensagem para um lugar
  String msg = "";
  if(Serial.available() > 0 ){
    msg = Serial.readStringUntil('\n'); //avalia as letras escritas ate encontrar \n assim colocando dentro de 'mensagem'
    Serial.print("mensagem digitada: ");
    Serial.println(msg);
    
    mqtt.publish("TopicoVitor", msg.c_str()); // envia a mensagem 
  }
  mqtt.loop();
}

void callback(char* topic, byte* payload, unsigned long length){
  String msgRecebida = "";
  for(int i = 0; i< length; i++){
    msgRecebida += (char) payload[i];
  }
  Serial.println(msgRecebida);
  //Fazer o controle aqui

  if(msgRecebida == "acender" ) {
    digitalWrite(ledPin, HIGH); 
    delay(1000);      
    Serial.println("ligando...");         
  }

  if(msgRecebida == "apagar" ) {              
    digitalWrite(ledPin, LOW);  
    delay(1000); 
    Serial.println("apagando...");
  }
}/configura a placa para mostrar na tela
  WiFi.begin(SSID, PASS);  // tenta conectar na rede
  Serial.println("Conectando no WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado com sucesso!");

  //Configura a placa para mostra na tela
  mqtt.setServer(brokerURL.c_str(), brokerPort);
  Serial.println("Conectando no Broker");
  
  String boardID = "S1-"; //Cria um nome que começa com "s1-"
  boardID += String(random(0xffff),HEX); //Junta o "s1-" com um número aleatório Hexadecimal

  //Enquanto não estiver conectado mostra "."
  while (!mqtt.connect(boardID.c_str())){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado com sucesso ao broker");
}

void loop() {
  String msg = "alguma coisa ;D";//texto com informação enviada para o broker
  Sring topico = "AulaIot/msg";
  mqtt.publish(topico.c_str(), msg.c_str());
  delay(20000);
}