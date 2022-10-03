/*
   Ambiente principal do algoritmo reservado para a
   execução basica das atividade do modulo, realizando
   o controle das conexões, medições, calculos e publicações
   encontrados nas demais partes do algoritmo.
*/

#include <Caixa_display.cpp>
#include <Caixa_Valores.cpp>
#include <Caixa_conectividade.cpp>
#include <Caixa_NTP.cpp>
#include <Caixa_publica_MQTT.cpp>

// --- WEBSERIAL ---
#include <WebSerial.h>

// ---MDNS ---
#include <ESPmDNS.h>

// --- OTA ---
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);

// --- LCD ---
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 20, 4);
#define dispAlterBotao 19
int displayPosition = 1;
int inicioContAltLCD;
int inicioContDesLCD;
bool alteraLCD = HIGH;
bool estadoBackLight = HIGH;
bool primDisp = HIGH;
String varTempoBotao;

// --- WIFI ---
#include <WiFi.h>
//Credenciais IsisWiFiZone
//const char* ssid = "IsisWiFiZone";
//const char* password = "8xz1CsJ@$M";

//Credenciais Casa
//const char* ssid = "Caio Victor";
//const char* password = "caio1106";

//Credenciais Celular
//const char* ssid = "Caio";
//const char* password = "20021106";

//Credenciais Roteador
const char* ssid = "SZA-IOT";
const char* password = "c!6KPbLD";

//Credenciais IsisZone
//const char* ssid = "IsisZone";
//const char* password = "zxrUyAq7Nr";

WiFiClient nodemcuClient;

// --- MQTT ---
#include <PubSubClient.h>
//const char* mqtt_Broker = "broker.hivemq.com";
//const char* mqtt_Broker = "broker.emqx.io";
//const char* mqtt_Broker = "192.168.0.105";
//const char* mqtt_Broker = "10.0.0.100";
//const char* mqtt_Broker = "10.0.0.118";
const char* mqtt_Broker = "10.0.1.9";

const char* mqtt_ClientID = "monitor_caiovictorr1106_LBV_ISIS";
//const char* mqtt_ClientID = "camara_fria_userBoard";
PubSubClient client(nodemcuClient);

//Topicos da base original
const char* topicoPercentualCaixa = "isis/lbv/caixaDagua1/percentualCaixa";
const char* topicoPercentualCisterna = "isis/lbv/caixaDagua1/percentualCisterna";
const char* topicoLitragemCaixa = "isis/lbv/caixaDagua1/litragemCaixa";
const char* topicoLitragemCisterna = "isis/lbv/caixaDagua1/litragemCisterna";
const char* topicoSensorCaixa = "isis/lbv/caixaDagua1/distanciaCaixa";
const char* topicoSensorCisterna = "isis/lbv/caixaDagua1/distanciaCisterna";
const char* topicoBoiaCloro = "isis/lbv/caixaDagua1/nivelCloro";
const char* topicoEstadoMotor = "isis/lbv/caixaDagua1/estadoMotor";
const char* topicoTempoMotor = "isis/lbv/caixaDagua1/tempoMotor";

const char* topicoEpochMotor = "isis/lbv/caixaDagua1/epochMotor";
const char* topicoTempoBotao = "isis/lbv/caixaDagua1/tempoBotao";

// --- SENSOR ULTRASSONICO---
#define trigPinCaixa 32
#define echoPinCaixa 33
#define trigPinCisterna 25
#define echoPinCisterna 26
float distance;
float distanciaCaixa;
float distanciaCisterna;
float litragemCaixa;
float litragemCisterna;
float percentualCaixa;
float percentualCisterna;

// ---CLORO/MOTORES---
#define pinoBoiaCloro 13
#define pinoMotorUm 15
#define pinoMotorDois 02
String estadoCloro;
String estadoMotor;
int inicioContMotor;
int finalContMotor;
String tempoMotor;

// ---CLORO/MOTORES---
#include <NTPClient.h>
WiFiUDP ntpUDP;
//NTPClient ntp(ntpUDP);
NTPClient ntp(ntpUDP, "a.st1.ntp.br", -3 * 3600, 60000);
const String horariosReset[] = {"00:00:00", "00:00:01", "00:00:02", "00:00:03", "12:00:00", "12:00:01", "12:00:02", "12:00:03"};

// ---LEITURA---
int inicioContLeitura;  
int leituraPosition = 0;
int quantidadeLeituras = 10;
float vetorCaixaLeituras[10];
float vetorCisternaLeituras[10];

// ---BUZEER---
#define pinoBuzzer 18

// ---LED---
#define pinoLed 4

float maiorValorCaixa = 0;
float maiorValorCisterna = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  //displaySetup();//Caixa_display

  digitalWrite(pinoLed, HIGH);

  pinMode(trigPinCaixa, OUTPUT);
  pinMode(echoPinCaixa, INPUT);
  pinMode(trigPinCisterna, OUTPUT);
  pinMode(echoPinCisterna, INPUT);
  pinMode(pinoBoiaCloro, INPUT);
  pinMode(pinoMotorUm, INPUT);
  pinMode(pinoMotorDois, INPUT);
  pinMode(dispAlterBotao, INPUT);
  pinMode(pinoBuzzer, OUTPUT);
  pinMode(pinoLed, OUTPUT);

  leituraSetup();

  Serial.begin(115200);
  conectarWifi();//Caixa_conectividade
  client.setServer(mqtt_Broker, 1883);

  ntp.begin();
  //ntp.setTimeOffset(-10800);
  //ntp.update();
  ntp.forceUpdate();

  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();

  if (!MDNS.begin("caixadagua1")) {
    Serial.println("Error starting mDNS");
    return;
  }

  conectarMQTT();

  conectarOTA();

  medirMotores();
  publicarDados();
}

void loop() {

  //ntp.update();
  ntp.forceUpdate();

  if (WiFi.status() != WL_CONNECTED) {
    reconectarWifi();
  }
  if (!client.connected()) {
    reconectarMQTT();//Caixa_conectividade
  }

  if (tempoLeitura()) {
    medirDistancia();//Caixa_valores
  }

  if (digitalRead(dispAlterBotao) == HIGH) {
    iniciaContAltLCD();
    iniciaContDesLCD();
    displayAltera();
    exibirDisplay(displayPosition);
    digitalWrite(pinoBuzzer, HIGH);
    delay(50);
    digitalWrite(pinoBuzzer, LOW);
    if (estadoBackLight == LOW) {
      lcd.backlight();
      estadoBackLight = HIGH;
    }
    if (digitalRead(dispAlterBotao) == HIGH) {
      //lerTempoBotao()
      //ntp.update();
      delay(500);
      if (digitalRead(dispAlterBotao) == HIGH) {
        lcd.init();
        iniciaContAltLCD();
        iniciaContDesLCD();
        exibirDisplay(displayPosition);
        delay(500);
        while (digitalRead(dispAlterBotao) == HIGH) {
          delay(250);
        }
      }
      //lerTempoBotao();
    }
    //break;
  } else if (alteraLCD and estadoBackLight) {
    exibirDisplay(displayPosition);
    displayAltera();
    alteraLCD = LOW;
  }
  digitalWrite(pinoLed, HIGH);
  delay(10);
  digitalWrite(pinoLed, LOW);
  delay(100);
  tempoAtivoLCD();
}
