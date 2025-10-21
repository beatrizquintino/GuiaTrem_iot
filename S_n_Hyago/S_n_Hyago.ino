#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;          //cria objeto p/ WiFi
PubSubClient mqtt(client);  //cria objeto p/ mqtt usando WiFi

const String brokerURL = "test.mosquitto.org";
const int brokerPort = 1883;

const String brokerUser = "";  //variável para o user do brocker
const String brokerPass = "";  //variável para a senha do brocker



const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(115200);    //configura a placa para mostrar na tela
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