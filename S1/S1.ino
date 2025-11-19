#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "env.h"
#include <DHT.h>

// ESP32Servo.h

#define DHTPIN 4
#define DHTTYPE DHT11

WiFiClientSecure client;          //cria objeto p/ WiFi
PubSubClient mqtt(client);  //cria objeto p/ mqtt usando WiFi

const byte trigg_pin = 22;
const byte echo_pin = 23;
const int ledPin = 19;
const int pino_LDR = 34;

DHT dht(DHTPIN, DHTTYPE);

int distancia = 0;
unsigned long tempo = 0;
float temperatura = 0;
float umidade = 0;


void setup() {
  Serial.begin(115200);    //configura a placa para mostrar na tela
  client.setInsecure();
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

  String boardID = "S1-";                  //Cria um nome que começa com "s1-"
  boardID += String(random(0xffff), HEX);  //Junta o "s1-" com um número aleatório Hexadecimal

  //Enquanto não estiver conectado mostra "."
  while (!mqtt.connected()) {
    Serial.print(".");
    mqtt.connect(boardID.c_str(), BROKER_USER_NAME, BROKER_USER_PASS);
    Serial.println(mqtt.state());
    delay(2000);
  }
  mqtt.subscribe(TOPIC_LUM);
  mqtt.setCallback(callback);
  Serial.println("\nConectado com sucesso ao broker");

  //DHT
  dht.begin();

  //presença
  pinMode(trigg_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(ledPin, OUTPUT);
}

long lerDistancia() {
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
  int luz = map(analogRead(pino_LDR), 0, 4095, 0, 100);
  char* luminosidade = "";
  if(luz > 95){
    luminosidade = "escuro";
  }else{
    luminosidade = "claro";
  }
  mqtt.publish(TOPIC_LUM, luminosidade);

  temperatura = dht.readTemperature(); 
  mqtt.publish(TOPIC_TEMP, String(temperatura).c_str());
  
  umidade = dht.readHumidity();
  mqtt.publish(TOPIC_UMID, String(umidade).c_str());

  //DHT
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println("%");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");

  //presença
  long distancia = lerDistancia();
  
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  if (distancia < 10) {
    mqtt.publish(TOPIC_PRESENCA,"Detectado");
  }

  mqtt.loop();  // Mantem conexão
  delay(500);
}

// Criação de função abaixo do loop
void callback(char* topic, byte* payload, unsigned long length) {  //Topíco/ Mensagem / Tamanho da mensagem
  String msgRecebida = "";
  for (int i = 0; i < length; i++) {
    msgRecebida += (char)payload[i];
  }
  Serial.println(msgRecebida);

  if (String(topic) == String(TOPIC_LUM)) {
    if (msgRecebida == "escuro") {
      digitalWrite(ledPin, HIGH);
      delay(1000);
      Serial.println("ligando...");
    }

    if (msgRecebida == "claro") {
      digitalWrite(ledPin, LOW);
      delay(1000);
      Serial.println("apagando...");
    }
  }
}