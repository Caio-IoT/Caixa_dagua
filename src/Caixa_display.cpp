/*
   Ambiente reservado para a apresentação dos valores captados
   pelo sensor ultrassonico e calculados em "Caixa_valores" em um
   display LCD com dimensões definidas em "Caixa_dagua_codigo".
*/

void exibirDisplay(int valor) {

  String espacDispNivel = "";
  String espacDispVolume = "";
  String espacDispPercentual = "";

  switch (valor) {
    case 1:
      if (litragemCaixa < 10000 and litragemCaixa > 1000) {
        espacDispVolume = " ";
      } else if (litragemCaixa < 1000 and litragemCaixa > 100) {
        espacDispVolume = "  ";
      } else if (litragemCaixa < 100 and litragemCaixa > 10) {
        espacDispVolume = "   ";
      } else if (litragemCaixa < 10) {
        espacDispVolume = "    ";
      }

      if (percentualCaixa < 100 and percentualCaixa > 10) {
        espacDispPercentual = " ";
      } else if (percentualCaixa < 10) {
        espacDispPercentual = "  ";
      }

      if (distanciaCaixa < 100 and distanciaCaixa > 10) {
        espacDispNivel = " ";
      } else if (distanciaCaixa < 10) {
        espacDispNivel = "  ";
      }

      lcd.setCursor(0, 0);
      lcd.print("====CAIXA D'AGUA====");
      lcd.setCursor(0, 1);
      lcd.print("|VOLUME:     " + espacDispVolume + (String)((int)litragemCaixa) + "L|");
      lcd.setCursor(0, 2);
      lcd.print("|PERCENTUAL:   " + espacDispPercentual + (String)((int)percentualCaixa) + "%|");
      lcd.setCursor(0, 3);
      lcd.print("|NIVEL:       " + espacDispNivel + (String)((int)distanciaCaixa) + "CM|");
      break;
    case 2:
      if (litragemCisterna < 10000 and litragemCisterna > 1000) {
        espacDispVolume = " ";
      } else if (litragemCisterna < 1000 and litragemCisterna > 100) {
        espacDispVolume = "  ";
      } else if (litragemCisterna < 100 and litragemCisterna > 10) {
        espacDispVolume = "   ";
      } else if (litragemCisterna < 10) {
        espacDispVolume = "    ";
      }

      if (percentualCisterna < 100 and percentualCisterna > 10) {
        espacDispPercentual = " ";
      } else if (percentualCisterna < 10) {
        espacDispPercentual = "  ";
      }

      if (distanciaCisterna < 100 and distanciaCisterna > 10) {
        espacDispNivel = " ";
      } else if (distanciaCisterna < 10) {
        espacDispNivel = "  ";
      }

      lcd.setCursor(0, 0);
      lcd.print("======CISTERNA======");
      lcd.setCursor(0, 1);
      lcd.print("|VOLUME:     " + espacDispVolume + (String)((int)litragemCisterna) + "L|");
      lcd.setCursor(0, 2);
      lcd.print("|PERCENTUAL:   " + espacDispPercentual + (String)((int)percentualCisterna) + "%|");
      lcd.setCursor(0, 3);
      lcd.print("|NIVEL:       " + espacDispNivel + (String)((int)distanciaCisterna) + "CM|");
      break;
    case 3:
      int tamTempo = tempoMotor.length();
      String spacDispTempo;
      String spacDispMotor = "";
      String spacDispCloro = "";
      tamTempo = (11 - tamTempo);
      while (tamTempo > 0) {
        spacDispTempo = (String)(spacDispTempo + " ");
        tamTempo--;
      }
      if (estadoMotor == "MOTOR 1" or estadoMotor == "MOTOR 2") {
        spacDispMotor = "  ";
      }
      if (estadoCloro == "BOM"){
        spacDispCloro = " ";
      }
      

      lcd.setCursor(0, 0);
      lcd.print("===CLORO||MOTORES===");
      lcd.setCursor(0, 1);
      lcd.print("|NIVEL CLORO:  " + spacDispCloro + estadoCloro + "|");
      lcd.setCursor(0, 2);
      lcd.print("|MOTORES: " + spacDispMotor + estadoMotor + "|");
      lcd.setCursor(0, 3);
      lcd.print("|TEMPO: " + spacDispTempo + tempoMotor + "|");
      break;
  }
}

void displaySetup(int valor) {

  switch (valor) {
    case 0:
      lcd.setCursor(2, 0);
      lcd.print("MONITOR DE NIVEL");
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("CAIXA DAGUA/CISTERNA");
      break;
    case 2:
      lcd.setCursor(0, 2);
      lcd.print("INICIANDO O SISTEMA!");
      break;
    case 3:
      lcd.setCursor(4, 3);
      lcd.print("CARREGANDO");
      break;
  }
}

void displayLoading() {

  lcd.setCursor(14, 3);
  lcd.print(".");
  delay(150);
  lcd.setCursor(15, 3);
  lcd.print(".");
  delay(150);
  lcd.setCursor(16, 3);
  lcd.print(".");
  delay(150);
  lcd.setCursor(17, 3);
  lcd.print(".");
  delay(150);
  lcd.setCursor(18, 3);
  lcd.print(".");
}

void displayErroWiFi() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("!!!!! PROBLEMA !!!!!");
  lcd.setCursor(0, 1);
  lcd.print("ERROR: CONECTIVIDADE");
  lcd.setCursor(0, 2);
  lcd.print(" WIFI: DESCONECTADO ");
  lcd.setCursor(2, 3);
  lcd.print("RECONECTANDO");
}

void displayErroMQTT() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("!!!!! PROBLEMA !!!!!");
  lcd.setCursor(0, 1);
  lcd.print("ERROR: CONECTIVIDADE");
  lcd.setCursor(0, 2);
  lcd.print(" MQTT: DESCONECTADO ");
  lcd.setCursor(2, 3);
  lcd.print("RECONECTANDO");
}

void displayErroMotor() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("!!!!! PROBLEMA !!!!!");
  lcd.setCursor(0, 1);
  lcd.print("ERROR: ESTADO MOTOR ");
  lcd.setCursor(0, 2);
  lcd.print(" MOTORES DESLIGADOS ");
  lcd.setCursor(0, 3);
  lcd.print("CAIXA DAGUA SECANDO!");
}

void displayAltera() {

  if (displayPosition == 3) {
    displayPosition = 1;
  } else {
    displayPosition ++;
  }
}
