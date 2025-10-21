#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"

WiFiClient client;          //cria objeto p/ WiFi
PubSubClient mqtt(client);  //cria objeto p/ mqtt usando WiFi

const byte ledPin = 2;

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;
const String topico = "TopicoBruno";

const String brokerUser = "";  //variável para o user do brocker
const String brokerPass = "";  //variável para a senha do brocker


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
  while (!mqttClient.connected()){
    mqttClient.connect(userId.c_str(), BROKER_USR_NAME, BROKER_USR_PASS)
    Serial.print(".");
    delay(2000);
  }

  mqtt.subscribe(TOPIC1);
  mqtt.setCallback(callback);
  Serial.println("\nConectado com sucesso ao broker");

}

void loop() {
  String mensagem = "";   //texto com informação enviada para o broker
  // Sring topico = "AulaIot/msg";
  // mqtt.publish(topico.c_str(), msg.c_str());
  // delay(20000);
  // mqtt.loop(); 

  if(Serial.available() > 0){
    mensagem = Serial.readStringUntil('\n');
    Serial.print("Mensagem digitada: ");
    Serial.println(mensagem);
    mqtt.publish("TopicoBeatriz",mensagem.c_str()); //envia msg
  }
  mqtt.loop(); //mantem a conexão
}

void callback(char* topic, byte* payload, unsigned long length){
    String mensagemRecebida = "";
    for(int i = 0; i < length; i++){
      mensagemRecebida += (char) payload[i];
    }
    Serial.println(mensagemRecebida);

  if(mensagemRecebida == "acender" ) {
    digitalWrite(ledPin, HIGH); 
    delay(1000);      
    Serial.println("ligando...");         
  }

  if(mensagemRecebida == "apagar" ) {              
    digitalWrite(ledPin, LOW);  
    delay(1000); 
    Serial.println("apagando...");
  }

}