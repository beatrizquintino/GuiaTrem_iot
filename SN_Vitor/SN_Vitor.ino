#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include "env.h"


WiFiClientSecure client;          //cria objeto p/ WiFi
PubSubClient mqtt(client);  //cria objeto p/ mqtt usando WiFi

int distancia = 0;
unsigned long tempo = 0;

const byte ledPin = 21;
const byte trigg_pin = 22;
const byte echo_pin = 23;

const int servo1Pin = 18;  // Pino GPIO 18 para o Servo 1
const int servo2Pin = 19;  // Pino GPIO 19 para o Servo 2

Servo servo1;
Servo servo2;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);  //configura a placa para mostrar na tela
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
  mqtt.setCallback(callback);
  Serial.println("Conectando no Broker");

  String userId = "S1-";                  //Cria um nome que começa com "s1-"
  userId += String(random(0xffff), HEX);  //Junta o "s1-" com um número aleatório Hexadecimal

  //Enquanto não estiver conectado mostra "."
  while (!mqtt.connected()) {
    Serial.print(".");
    mqtt.connect(userId.c_str(), BROKER_USER_NAME, BROKER_USER_PASS);
    delay(2000);
  }

  pinMode(trigg_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  mqtt.subscribe(TOPIC_LUM);
  mqtt.subscribe(TOPIC_PRESENCA_1);
  mqtt.subscribe(TOPIC_PRESENCA_2);
  mqtt.subscribe(TOPIC_PRESENCA_3);
  mqtt.setCallback(callback);
  Serial.println("\nConectado com sucesso ao broker");

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
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

  long distancia = lerDistancia();

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (distancia < 10) {
    mqtt.publish(TOPIC_PRESENCA_3, "Detectado");
  }

  mqtt.loop();  // Mantem conexão
  delay(500);
}

void callback(char* topic, byte* payload, unsigned long length) {
  String mensagemRecebida = "";
  for (int i = 0; i < length; i++) {
    mensagemRecebida += (char)payload[i];
  }
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  Serial.println(mensagemRecebida);

  if (strcmp(topic, TOPIC_LUM) == 0 && mensagemRecebida == String("escuro")) {
    digitalWrite(ledPin, HIGH);
    Serial.println("ligando...");
  } else if (topic == TOPIC_LUM && mensagemRecebida == "claro") {
    digitalWrite(ledPin, LOW);
    Serial.println("apagando...");
  }

  if (strcmp(topic, TOPIC_PRESENCA_1) == 0) {
    if (mensagemRecebida == "1") {
      servo1.write(90);  // Move o servo para 90 graus
      Serial.println("Servo 1 acionado (90 graus)");
      mqtt.publish(TOPIC_SERVO_1, "90");
    }
  }

  if (strcmp(topic, TOPIC_PRESENCA_2) == 0) {
    if (mensagemRecebida == "1") {
      servo2.write(90);  // Move o servo para 90 graus
      Serial.println("Servo 2 acionado (90 graus)");
      mqtt.publish(TOPIC_SERVO_2, "90");
    }
  }


  if (strcmp(topic, TOPIC_PRESENCA_3) == 0) {
    if (mensagemRecebida == "Detectado") {
      servo1.write(120);  // Move o servo para 90 graus
      Serial.println("Servo 1 acionado (120 graus)");
      servo2.write(120);  // Move o servo para 90 graus
      Serial.println("Servo 2 acionado (120 graus)");
      mqtt.publish(TOPIC_SERVO_1, "120");
        mqtt.publish(TOPIC_SERVO_2, "120");
    }
  }

}