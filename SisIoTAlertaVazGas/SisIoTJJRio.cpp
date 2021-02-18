#include "SisIoTJJRio.h"

SisIoTJJRio::SisIoTJJRio(){ }

SisIoTJJRio::SisIoTJJRio(SisIoTJJRDados& _dados, SisIoTJJRTools&  _tools, SisIoTJJRDataHora&  _dh, SisIoTJJRSensorGas&  _mq5, SisIoTJJRMQTT& _mqtt, SisIoTJJRTelegram& _gasBot ){
  this->dados  = &_dados;
  this->tools   = &_tools;
  this->dh      = &_dh;
  this->mq5    = &_mq5;
  this->mqtt   = &_mqtt;
  this->gasBot = &_gasBot;
}


bool SisIoTJJRio::postarTelegram(String payOUT ) {
  return gasBot->postarTelegram(payOUT); 
}

void SisIoTJJRio::publicarMQTT(String topOUT, String subOUT, String payOUT ) {
  mqtt->publicarTopico(topOUT, subOUT, payOUT);
}

//ThingSpeak *****************************************
void SisIoTJJRio::postarThingSpeak(float sensorVolt){
  int conectar = dados->client.connect(this->_serverTS, this->_portaTS);  
  // "184.106.153.149" or api.thingspeak.com
  Serial.println("Conectando: "+String(conectar));
  if (conectar){
    Serial.println("Post Thingspeak...");   
    String urlPost = this->_apiKeyTS;
    urlPost.concat("&field1=");
    urlPost.concat(String(sensorVolt)); 
    urlPost.concat("\r\n'\'");     
    dados->client.print("POST /update HTTP/1.1\n");
    dados->client.print("Host: api.thingspeak.com\n");
    dados->client.print("Connection: close\n");
    dados->client.print("X-THINGSPEAKAPIKEY: " + this->_apiKeyTS + "\n");
    dados->client.print("Content-Type: application/x-www-form-urlencoded\n");
    dados->client.print("Content-Length: ");
    dados->client.print(urlPost.length());
    dados->client.print("\n\n");
    dados->client.print(urlPost);
    Serial.print("\nMedidor de Gás ");
    Serial.print(String(sensorVolt));
    Serial.println("V");
    Serial.println("---> Enviado Thingspeak.");
  }
  dados->client.stop();
  Serial.println("Esperando...");  
  // thingspeak precisa de um atraso mínimo de 15 segundos entre as atualizações, eu configurei para 30 segundos
}

//*** MSG *********************************************************
String SisIoTJJRio::msgDinamica(){  
  dados->payloadDpy = "MQTT conectado";//msg padrao
  
  if(mq5->getVazamentoVUltimo()!=0 && dados->alternarMsgVaz){
    dados->payloadDpy        = "Vazamento "+dados->_dataVazUltimo;
    dados->alternarMsgVaz    = false;    
  }else if(mq5->getVazamentoVUltimo()!=0){  
    dados->payloadDpy        = String(mq5->getVazamentoVUltimo())+"V às "+dados->_horaVazUltimo;
    dados->alternarMsgVaz    = true;
  }
  return dados->payloadDpy;
}

String SisIoTJJRio::atualizarMsgAlerta(){
  dados->msgAlerta = dados->_msgAlertaCabecalho+": ALERTA DE VAZAMENTO!!!";
  String msgCorpo;
  msgCorpo.concat("\nZona 1 - Cozinha");
  msgCorpo.concat("\nSSID: ");
  msgCorpo.concat(_SSID);
  msgCorpo.concat(", IP: ");
  msgCorpo.concat(tools->getIp());
  msgCorpo.concat("\n Sensor: "+String(mq5->getSensorV())+"V\n"+dh->getHora()+" "+dh->getDataBR());
  dados->msgAlerta += msgCorpo +"\nLink do Gráfico:\n"+dados->urlTS; 

  return dados->msgAlerta;
}
