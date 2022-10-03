/*
   Ambiente reservado para realização das medidas atraves
   do sensor ultrssonico e calculo de todos os valores
   resultantes dos valores adquiridos.
*/

#include <Caixa_dagua_codigo.cpp>

// --- DIMENSOES CAIXA/CISTERNA ---
float raioCaixa = 296 / 2; //calculando raio da caixa com o diametro
float raioCisterna = raioCaixa; // raio e diametro identico ao da caixa
float distanciaLaminaCaixa = 22;//35
float distanciaLaminaCisterna = 7.5;
float toleranciaLeitura = 2;
float alturaMaximaCaixa = 202.83;//200 e 212.67 e 225.46 e 251.41
float alturaMaximaCisterna = 154.73;//190 e 160 e 175.67 e 154.73
float volumeTotalCaixa = (3.14159 * (raioCaixa * raioCaixa) * alturaMaximaCaixa) / 1000; //Calcula o volume maximo da caixa d'águas
float volumeTotalCisterna = (3.14159 * (raioCisterna * raioCisterna) * alturaMaximaCisterna) / 1000; //Calcula o volume maximo da Cisterna

// --- CONTROLE DO TEMPO DOS MOTORES ---
String motorLigado;

void medirDistancia() {

  float medeDistanciaCaixa;
  float medeDistanciaCisterna;

  delayMicroseconds(10);
  digitalWrite(trigPinCaixa, HIGH);
  delayMicroseconds(40);
  digitalWrite(trigPinCaixa, LOW);
  long durationCaixa = pulseIn(echoPinCaixa, HIGH);
  medeDistanciaCaixa = durationCaixa * 0.034 / 2;
  medeDistanciaCaixa = alturaMaximaCaixa - (medeDistanciaCaixa - distanciaLaminaCaixa);
  //medeDistanciaCaixa = 100.0;

  delayMicroseconds(10);
  digitalWrite(trigPinCisterna, HIGH);
  delayMicroseconds(40);
  digitalWrite(trigPinCisterna, LOW);
  long durationCisterna = pulseIn(echoPinCisterna, HIGH);
  medeDistanciaCisterna = durationCisterna * 0.034 / 2;
  medeDistanciaCisterna = alturaMaximaCisterna - (medeDistanciaCisterna - distanciaLaminaCisterna);
  //medeDistanciaCisterna = 100.0;

  vetorCaixaLeituras[leituraPosition] = medeDistanciaCaixa;
  vetorCisternaLeituras[leituraPosition] = medeDistanciaCisterna;
  leituraAltera();
}

void calcularValores() {
  float somaLeiturasCaixa;
  float somaLeiturasCisterna;

  int countSemelhantesCaixa = 0;
  float vetoresSemelhantesCaixa[10];
  float valorComparadorCaixa;
  int posValorComparadorCaixa = 0;

  int countSemelhantesCisterna = 0;
  float vetoresSemelhantesCisterna[10];
  float valorComparadorCisterna;
  int posValorComparadorCisterna = 0;

  float bckpDistanciaCaixa = distanciaCaixa;
  float bckpDistanciaCisterna = distanciaCisterna;
  float bckpPercentualCaixa = percentualCaixa;
  float bckpPercentualCisterna = percentualCisterna;
  float bckpLitragemCaixa = litragemCaixa;
  float bckpLitragemCisterna = litragemCisterna;

  float copiaCisterna[10];
  Serial.print("\nArray Cisterna: {");
  for (int i = 0; i <= 9; i++) {
    copiaCisterna[i] = vetorCisternaLeituras[i];
    Serial.print(vetorCisternaLeituras[i]);
    if (i != 9) {
      Serial.print(", ");
    }
  }
  Serial.print("}\n");

  for (int i = 0; i <= (sizeof(copiaCisterna) / sizeof(float)) - 1; i++) {
    //Serial.println("EXECUTOU CAIXA");
    if (countSemelhantesCisterna == 0) {
      valorComparadorCisterna = copiaCisterna[posValorComparadorCisterna];
      posValorComparadorCisterna += 1;
      vetoresSemelhantesCisterna[countSemelhantesCisterna] = valorComparadorCisterna;
      countSemelhantesCisterna = 1;
      Serial.print("ADICIONADO CISTERNA: ");
      Serial.println(valorComparadorCisterna);
    }
    if (abs(valorComparadorCisterna - copiaCisterna[i + 1]) <= toleranciaLeitura and copiaCisterna[i + 1] > 0) {
      vetoresSemelhantesCisterna[countSemelhantesCisterna] = copiaCisterna[i + 1];
      Serial.print("ADICIONADO CISTERNA: ");
      Serial.println(copiaCisterna[i + 1]);
      countSemelhantesCisterna += 1;
    }
    if (i == 9 and countSemelhantesCisterna < 5) {
      i = 0;
      countSemelhantesCisterna = 0;
      memset(vetoresSemelhantesCisterna, -1 , sizeof(vetoresSemelhantesCisterna));
    }
  }


  for (int i = 0; i <= countSemelhantesCisterna - 1; i++) {
    //Serial.println(i);
    if (vetoresSemelhantesCisterna[i] != -1) {
      somaLeiturasCisterna += vetoresSemelhantesCisterna[i];
      //Serial.print("Caixa: ");
      //Serial.println(vetoresSemelhantesCaixa[i]);
      Serial.println(vetoresSemelhantesCisterna[i]);
      //Serial.print(", ");
    }
  }
  distanciaCisterna = somaLeiturasCisterna / countSemelhantesCisterna;


  float copiaCaixa[10];
  Serial.print("\nArray Caixa: {");
  for (int i = 0; i <= 9; i++) {
    copiaCaixa[i] = vetorCaixaLeituras[i];
    Serial.print(vetorCaixaLeituras[i]);
    if (i != 9) {
      Serial.print(", ");
    }
  }
  Serial.print("}\n");

  for (int i = 0; i <= (sizeof(copiaCaixa) / sizeof(float)) - 1; i++) {
    //Serial.println("EXECUTOU CAIXA");
    if (countSemelhantesCaixa == 0) {
      valorComparadorCaixa = copiaCaixa[posValorComparadorCaixa];
      posValorComparadorCaixa += 1;
      vetoresSemelhantesCaixa[countSemelhantesCaixa] = valorComparadorCaixa;
      countSemelhantesCaixa = 1;
      Serial.print("ADICIONADO CAIXA: ");
      Serial.println(valorComparadorCaixa);
    }
    if (abs(valorComparadorCaixa - copiaCaixa[i + 1]) <= toleranciaLeitura and copiaCaixa[i + 1] > 0) {
      vetoresSemelhantesCaixa[countSemelhantesCaixa] = copiaCaixa[i + 1];
      Serial.print("ADICIONADO CAIXA: ");
      Serial.println(copiaCaixa[i + 1]);
      countSemelhantesCaixa += 1;
    }
    if (i == 9 and countSemelhantesCaixa < 5) {
      i = 0;
      countSemelhantesCaixa = 0;
      memset(vetoresSemelhantesCaixa, -1 , sizeof(vetoresSemelhantesCaixa));
    }
  }
  for (int i = 0; i <= countSemelhantesCaixa - 1; i++) {
    //Serial.println(i);
    if (vetoresSemelhantesCaixa[i] != -1) {
      somaLeiturasCaixa += vetoresSemelhantesCaixa[i];
      //Serial.print("Caixa: ");
      //Serial.println(vetoresSemelhantesCaixa[i]);
      Serial.println(vetoresSemelhantesCaixa[i]);
      //Serial.print(", ");
    }
  }
  distanciaCaixa = somaLeiturasCaixa / countSemelhantesCaixa;

  if (distanciaCaixa > 300 or distanciaCisterna > 300) {
    distanciaCaixa = bckpDistanciaCaixa;
    distanciaCisterna = bckpDistanciaCisterna;
  }

  litragemCaixa = (3.14159 * (raioCaixa * raioCaixa) * distanciaCaixa) / 1000;
  litragemCisterna = ((3.14159 * (raioCisterna * raioCisterna)) * distanciaCisterna) / 1000;
  percentualCaixa = (100 / volumeTotalCaixa) * litragemCaixa;
  percentualCisterna = (100 / volumeTotalCisterna) * litragemCisterna;

  if (percentualCaixa < 0 or percentualCisterna < 0 or (abs(percentualCaixa - bckpPercentualCaixa) > 15 and bckpPercentualCaixa > 0) or (abs(percentualCisterna - bckpPercentualCisterna) > 15 and bckpPercentualCisterna)) {
    distanciaCaixa = bckpDistanciaCaixa;
    distanciaCisterna = bckpDistanciaCisterna;
    litragemCaixa = bckpLitragemCaixa;
    litragemCisterna = bckpLitragemCisterna;
    percentualCaixa = bckpPercentualCaixa;
    percentualCisterna = bckpPercentualCisterna;
  }
}

void medirCloro() {
  bool estadoBoiaCloro = digitalRead(pinoBoiaCloro);
  if (!estadoBoiaCloro) {
    estadoCloro = "BOM";
  } else {
    estadoCloro = "RUIM";
  }
}

void medirMotores() {
  bool estadoMotorUm = digitalRead(pinoMotorUm);
  bool estadoMotorDois = digitalRead(pinoMotorDois);
  if (estadoMotorUm == LOW) {
    estadoMotor = "MOTOR 1";
  } else if (estadoMotorUm == HIGH) {
    estadoMotor = "DESLIGADO";
    if (litragemCaixa < 2000.0 and litragemCisterna > 2000.0) {
      displayErroMotor();
      digitalWrite(pinoLed, HIGH);
      digitalWrite(pinoBuzzer, HIGH);
      while (!digitalRead(pinoMotorUm) and !digitalRead(pinoMotorDois)) {
        medirDistancia();
        calcularValores();
        if (digitalRead(pinoMotorUm) or digitalRead(pinoMotorDois) or litragemCaixa >= 2000.0) {
          digitalWrite(pinoLed, LOW);
          digitalWrite(pinoBuzzer, LOW);
          break;
        }
      }
    }
  }

  if (estadoMotor != "DESLIGADO") {
    if (estadoMotor != motorLigado) {
      iniciaContagem();
      motorLigado = estadoMotor;
    }
    finalizaContagem();
    int calcTempoMotor = finalContMotor - inicioContMotor;

    int horas = (calcTempoMotor / 3600);
    int minutos = (calcTempoMotor - (3600 * horas)) / 60;
    int segundos = (calcTempoMotor - (3600 * horas) - (minutos * 60));
    int dias = horas / 24;
    horas = horas % 24;

    tempoMotor = "";
    String zeroStrg = "";
    if (horas == 0 and dias == 0) {
      if (segundos <= 9) {
        zeroStrg = "0";
      }
      tempoMotor = zeroStrg + ((String)segundos) + "s";
    }
    if (minutos >= 1 or (horas >= 1 or dias >= 1)) {
      zeroStrg = "";
      if (minutos <= 9) {
        zeroStrg = "0";
      }
      tempoMotor = zeroStrg + ((String)minutos) + "m" + tempoMotor;
    }
    if (horas >= 1 or (dias >= 1)) {
      zeroStrg = "";
      if (horas <= 9) {
        zeroStrg = "0";
      }
      tempoMotor = zeroStrg + ((String)horas) + "h" + tempoMotor;
    }
    if (dias >= 1) {
      zeroStrg = "";
      if (dias <= 9) {
        zeroStrg = "0";
      }
      tempoMotor = zeroStrg + ((String)dias) + "d" + tempoMotor;
    }

  } else {
    motorLigado = "DESLIGADO";
    tempoMotor = "-----";
  }
}
void leituraAltera() {

  if (leituraPosition == 9) {
    leituraPosition = 0;
    calcularValores();
    medirCloro();
    medirMotores();
    publicarDados();//Caixa_publica_MQTT
  } else {
    leituraPosition ++;
  }
}

void leituraSetup() {

  for (int i = 0; i <= 9; i++) {

    if (i >= 9) {
      displayLoading();
    } else {
      displaySetup(i);
    }

    delayMicroseconds(10);
    digitalWrite(trigPinCaixa, HIGH);
    delayMicroseconds(40);
    digitalWrite(trigPinCaixa, LOW);
    long durationCaixa = pulseIn(echoPinCaixa, HIGH);
    distanciaCaixa = durationCaixa * 0.034 / 2;
    distanciaCaixa = alturaMaximaCaixa - (distanciaCaixa - distanciaLaminaCaixa);
    //distanciaCaixa = 100.0;

    delayMicroseconds(10);
    digitalWrite(trigPinCisterna, HIGH);
    delayMicroseconds(40);
    digitalWrite(trigPinCisterna, LOW);
    long durationCisterna = pulseIn(echoPinCisterna, HIGH);
    distanciaCisterna = durationCisterna * 0.034 / 2;
    distanciaCisterna = alturaMaximaCisterna - (distanciaCisterna - distanciaLaminaCisterna);
    //distanciaCisterna = 100.0;

    vetorCaixaLeituras[i] = distanciaCaixa;
    vetorCisternaLeituras[i] = distanciaCisterna;
    delay(450);
  }
  calcularValores();
  medirCloro();
}
