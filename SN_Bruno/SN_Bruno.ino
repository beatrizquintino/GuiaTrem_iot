#include <WiFi.h>
const String SSID = "nome da rede";
const String PASS = "senha da rede";

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);
  Serial.println("Conectando no WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("Conectado com sucesso");
}

void loop() {
  // put your main code here, to run repeatedly:

}
