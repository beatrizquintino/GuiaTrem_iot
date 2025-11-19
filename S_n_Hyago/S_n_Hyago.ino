#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"

WiFiClientSecure client;          //cria objeto p/ WiFi
PubSubClient mqtt(client);  //cria objeto p/ mqtt usando WiFi

const byte ledPin = 2;

int distancia = 0;
unsigned long tempo = 0;

//sensores s1
const byte trigg_pin_1= 21;
const byte echo_pin_1= 19;

//sensores s2
const byte trigg_pin_2= 25;
const byte echo_pin_2= 27;


void setup() {
//led  
  pinMode(ledPin, OUTPUT);

//sensores  
  pinMode(trigg_pin_1, OUTPUT);
  pinMode(echo_pin_1, INPUT);
  //repetir para o 2
  
  client.setInsecure();                                                                  
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
    mqtt.connect(boardID.c_str(), BROKER_USR_NAME, BROKER_USR_PASS);
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(TOPIC_LUM); //vamos mudar (topico)
  mqtt.setCallback(callback);
  Serial.println("\nConectado com sucesso ao broker");
}

long lerDistancia(int trigg_pin, int echo_pin) {
  digitalWrite(trigg_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigg_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigg_pin, LOW);
  
  long duracao = pulseIn(echo_pin, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  
  return distancia;
}

void loop() {
  //ler ultrassonico 1
  long distancia_1 = lerDistancia(trigg_pin_1, echo_pin_1);
  Serial.printf("Distância: %.2fcm\n",distancia_1);
  
  //se <10 publica
  if (distancia_1 < 10) {
    mqtt.publish(TOPIC_PRESENCA_1, "Detectado"); 
  }


  //ler ultrassonico 2
  long distancia_2 = lerDistancia(trigg_pin_2, echo_pin_2);
  Serial.printf("Distância: %.2fcm\n",distancia_2);
  //se <10 publica
  if (distancia_2 < 10) {
  mqtt.publish(TOPIC_PRESENCA_2, "Detectado"); 
  
  }
  

  mqtt.loop();  // Mantem conexão
  delay(500);
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
}