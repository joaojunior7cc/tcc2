#include "SisIoTJJRDBGas.h"//versao 8

//Back-End
SisIoTJJRDados      dados;
SisIoTJJRTools      tools; 
SisIoTJJRDataHora   dh(dados);
SisIoTJJRAutomocao  autom(dados);
SisIoTJJRSensorGas  mq5(dados);
SisIoTJJRMQTT       mqtt(dados);
SisIoTJJRTelegram   gasBot(dados);
SisIoTJJRio         io(dados,tools,dh,mq5,mqtt,gasBot);
//Front-End
SisIoTJJRDBGas      dbgas(dados,dh,autom,mq5,tools,io);//DashBoard  Gas

void setup() {
  Serial.begin(115200);
  Serial.println("setup(ENTROU)");
  Serial.print("Conectando...");
  Serial.println(_SSID);
  tools.conectarWifi();
  Serial.println("\nWiFi conectado " + tools.getIp());
  gasBot.certificado();//esta versao exige
  gasBot.tempoRecuperacao();//tempo padrao de espera da lib (UniversalTelegramBot) necessario para o Telegram funcionar
  gasBot.setComandos();//setando quais comando o Bot ira exibir no Telegram
  autom.configurarPinos();
  mqtt.configurarMQTT();
  mqtt.verificarMQTT();
  mq5.setLimiteV(7.11);//novo limite
  Serial.println("setup(SAIU)");
}

void loop() {
  float adc = analogRead(A0); //lendo pino Analogico do Sensor que informa o nivel de concentração de gases detectados.
  mq5.setSensorV(adc);//calcula o V,   
  alimentar();

  dbgas.alimentar("");//dashboard gas

  gasBot.lerTelegram();
             
  //reset
  if (autom.getResetar()) { autom.setResetar(false); setup(); }
  tools.delay2(5000);
  ESP.wdtFeed();//Alimenta o Watchdog.
}

//METODOS ***************************************************************************
void alimentar(){
  dh.atualizarDataHora();
  io.atualizarMsgAlerta();
  dh.setHHmmss(dados._hora);//separando h m s
}
