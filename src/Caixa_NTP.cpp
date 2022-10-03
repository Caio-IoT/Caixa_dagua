/*
   Ambiente reservado para a realização das contagens
   dos tempos dos motores para ser enviado na comunicação da placa".
*/

#include <Caixa_dagua_codigo.cpp>

void iniciaContagem() {
  String strgInicioCont = (String)ntp.getEpochTime();
  inicioContMotor = strgInicioCont.toInt();
}

void finalizaContagem() {
  String strgFinalCont = (String)ntp.getEpochTime();
  finalContMotor = strgFinalCont.toInt();
}

void iniciaContAltLCD() {
  String strgInicioCont = (String)ntp.getEpochTime();
  inicioContAltLCD = strgInicioCont.toInt();
}

void iniciaContDesLCD() {
  String strgInicioCont = (String)ntp.getEpochTime();
  inicioContDesLCD = strgInicioCont.toInt();
}

void iniciaContLeitura() {
  String strgInicioCont = (String)ntp.getEpochTime();
  inicioContLeitura = strgInicioCont.toInt();
}

void tempoAtivoLCD() {
  String strgEpochTime = (String)ntp.getEpochTime();
  int epochTime = strgEpochTime.toInt();

  if (inicioContAltLCD == 0) {
    iniciaContAltLCD();
  }
  if (inicioContDesLCD == 0) {
    iniciaContDesLCD();
  }
  if ((epochTime - inicioContAltLCD) >= 10) {
    alteraLCD = HIGH;
    iniciaContAltLCD();
  }
  if ((epochTime - inicioContDesLCD) >= 90) {
    lcd.noBacklight();
    estadoBackLight = LOW;
    iniciaContDesLCD();
  }
}

bool tempoLeitura() {
  String strgEpochTime = (String)ntp.getEpochTime();
  int epochTime = strgEpochTime.toInt();
  if (inicioContLeitura == 0) {
    iniciaContLeitura();
    return true;
  }
  if ((epochTime - inicioContLeitura) >= 2 or leituraPosition == 0) {
    iniciaContLeitura();
    return true;
  }
  return false;
}

void lerTempoBotao() {

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  varTempoBotao = (&timeinfo, "%d/%m/%y - %T");
}

void tempoReset() {
  
  String hora = ntp.getFormattedTime();

  for (int i = (sizeof(horariosReset) / sizeof(String)); i >= 0; i--) {
    if (hora == horariosReset[i]) {
      ESP.restart();
    }
  }
}
