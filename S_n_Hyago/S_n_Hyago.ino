#include<WiFi.h>
const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

void setup() {
  Serial.begin(115200); //configura a plca para mostrar na tela
  WiFi.begin(SSID, PASS); //tenta conectar na rede
  Serial.println("conectando no wifi");
  while(WiFi.status() !=WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("conectado com sucesso!");
}

void loop() {


}
