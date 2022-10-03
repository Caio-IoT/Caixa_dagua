/*
   Ambiente reservado para a realização da publicação
   dos valores captados pelo sensor ultrassonico e calculados
   em "Caixa_valores" no broker MQTT selecionado em "Caixa_dagua_codigo".
*/

#include <Caixa_dagua_codigo.cpp>

void publicarDados() {
  //Publicação no broker MQTT
  client.publish(topicoLitragemCaixa, String(litragemCaixa).c_str(), true);
  client.publish(topicoLitragemCisterna, String(litragemCisterna).c_str(), true);
  client.publish(topicoPercentualCaixa, String(percentualCaixa).c_str(), true);
  client.publish(topicoPercentualCisterna, String(percentualCisterna).c_str(), true);
  client.publish(topicoSensorCaixa, String(distanciaCaixa).c_str(), true);
  client.publish(topicoSensorCisterna, String(distanciaCisterna).c_str(), true);
  client.publish(topicoBoiaCloro, String(estadoCloro).c_str(), true);
  client.publish(topicoEstadoMotor, String(estadoMotor).c_str(), true);
  client.publish(topicoTempoMotor, String(tempoMotor).c_str(), true);

  //Confirmação de envio no monitor Serial
  Serial.println("\nDados publicados no broker com sucesso!!");

  //Rascunho no monitor serial
  Serial.println("Litragem Caixa: " + (String)litragemCaixa + " Litros");
  Serial.println("Litragem Cisterna: " + (String)litragemCisterna + " Litros");
  Serial.println("Percentual Caixa: " + (String)percentualCaixa + " %");
  Serial.println("Percentual Cisterna: " + (String)percentualCisterna + " %");
  Serial.println("Sensor Distancia Caixa: " + (String)distanciaCaixa + " CM");
  Serial.println("Sensor Distancia Cisterna: " + (String)distanciaCisterna + " CM");
  Serial.println("Estado Cloro: " + estadoCloro);
  Serial.println("Estado Motor: " + estadoMotor);
  Serial.println("Tempo Motor: " + tempoMotor + "\n");
  
  if (distanciaCaixa > maiorValorCaixa) {
    maiorValorCaixa = distanciaCaixa;
  }
  if (distanciaCisterna > maiorValorCisterna) {
    maiorValorCisterna = distanciaCisterna;
  }

  //client.publish("testes/valorMaxCaixa", String(maiorValorCaixa).c_str(), true);
  //client.publish("testes/valorMaxCisterna", String(maiorValorCisterna).c_str(), true);

  //Rascunho no monitor WebSerial
  String hora = ntp.getFormattedTime();
  WebSerial.println("\n" + hora + "-Dados publicados no broker com sucesso!!");
  WebSerial.println(hora + "-Litragem Caixa: " + (String)litragemCaixa + " Litros");
  WebSerial.println(hora + "-Litragem Cisterna: " + (String)litragemCisterna + " Litros");
  WebSerial.println(hora + "-Percentual Caixa: " + (String)percentualCaixa + " %");
  WebSerial.println(hora + "-Percentual Cisterna: " + (String)percentualCisterna + " %");
  WebSerial.println(hora + "-Sensor Distancia Caixa: " + (String)distanciaCaixa + " CM");
  WebSerial.println(hora + "-Sensor Distancia Cisterna: " + (String)distanciaCisterna + " CM");
  WebSerial.println(hora + "-Estado Cloro: " + estadoCloro);
  WebSerial.println(hora + "-Estado Motor: " + estadoMotor);
  WebSerial.println(hora + "-Tempo Motor: " + tempoMotor + "\n");

  //teste provisorio

  WebSerial.println(hora + "-Valor Maximo Caixa: " + maiorValorCaixa + "\n");
  WebSerial.println(hora + "-Valor Maximo Cisterna: " + maiorValorCisterna + "\n");

}

void callback(char *topic, byte *payload, unsigned int length) {
  String nome;
  for (int i = 0; i < length; i++) {
    nome += (char) payload[i];
  }
  Serial.println(nome);
}
