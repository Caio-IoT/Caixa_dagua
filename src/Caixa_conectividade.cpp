/*
   Ambiente reservado para a realização das conexões
   dos dispositivos necessarios para a comunicação da placa,
   como os utilizados em "Caixa_dagua_codigo" e "Caixa_publica_MQTT".
*/

#include "Caixa_dagua_codigo.cpp"

void conectarWifi() {
  int tryCount = 0;
  Serial.print("\nConectando a ");
  Serial.println((String)ssid);
  Serial.println();

  WiFi.mode(WIFI_STA); //faz parte do OTA
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(14, 3);
    lcd.print("      ");
    delay(150);
    displayLoading();//Caixa_display
    tryCount ++;
    if (tryCount == 20) {
      displayErroWiFi();
      //tryCount = 0;
      digitalWrite(pinoBuzzer, HIGH);
    }
    if (tryCount == 40) {
      digitalWrite(pinoBuzzer, LOW);
      ESP.restart();
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado!!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(pinoBuzzer, LOW);
    digitalWrite(pinoLed, LOW);
  }
}

void reconectarWifi() {
  int tryCount = 0;
  Serial.print("\nConectando a ");
  Serial.println((String)ssid);
  Serial.println();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tryCount ++;
    if (tryCount == 20) {
      displayErroWiFi();
      digitalWrite(pinoBuzzer, HIGH);
    }
    if (tryCount == 40) {
      digitalWrite(pinoBuzzer, LOW);
      ESP.restart();
    }
    if (tryCount >= 20) {
      lcd.setCursor(14, 3);
      lcd.print("      ");
      delay(150);
      displayLoading();//Caixa_display
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado!!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(pinoBuzzer, LOW);
  }
}

void reconectarMQTT() {
  int tryCount = 0;
  Serial.print("\nConectando ao broker ");
  Serial.println((String)mqtt_Broker);
  Serial.println();
  while (!client.connected()) {
    client.connect(mqtt_ClientID);
    Serial.println("Reconectando");
  }
  if (client.connected()) {
    Serial.println("Reconcetado com sucesso!!");
  }
}

void conectarMQTT() {

  int tryCount = 0;
  while (!client.connected()) {
    client.connect(mqtt_ClientID);
    Serial.println("Conectando...");
    lcd.setCursor(14, 3);
    lcd.print("      ");
    delay(150);
    displayLoading();//Caixa_display
    tryCount ++;
    if (tryCount == 20) {
      displayErroMQTT();
      digitalWrite(pinoBuzzer, HIGH);
    }
    if (tryCount == 40) {
      digitalWrite(pinoBuzzer, LOW);
      ESP.restart();
    }
  }
  if (client.connected()) {
    Serial.println("Conectado com sucesso!!");
    digitalWrite(pinoBuzzer, LOW);
  }
}

void conectarOTA() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/plain", "Este e o Monitoramento da Caixa D'agua.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}

void recvMsg(uint8_t *data, size_t len) {
  WebSerial.println("Received Data...");
  String d = "";
  for (int i = 0; i < len; i++) {
    d += char(data[i]);
  }
  WebSerial.println(d);
  if (d == "ON") {
    //digitalWrite(LED, HIGH);
  }
  if (d == "OFF") {
    //digitalWrite(LED, LOW);
  }
}
