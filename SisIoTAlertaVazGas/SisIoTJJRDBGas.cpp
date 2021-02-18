#include "SisIoTJJRDBGas.h"

SisIoTJJRDBGas::SisIoTJJRDBGas(SisIoTJJRDados&     _dados, SisIoTJJRDataHora& _dh, SisIoTJJRAutomocao& _autom,
                               SisIoTJJRSensorGas& _mq5, SisIoTJJRTools&   _tools,  SisIoTJJRio&       _io  ){
  this->dados = &_dados;
  this->dh    = &_dh;
  this->autom = &_autom;
  this->mq5   = &_mq5; 
  this->tools = &_tools;
  this->io    = &_io;
}


void SisIoTJJRDBGas::lerMensagens(){
  //chegou request MQTT?
  if (dados->STATUSCALLBACK) {
    if(dados->topicoCall.equals("/gas")){
      executarPublicar(dados->topicoCall, dados->subtopicoCall, dados->payloadCall);
      dados->STATUSCALLBACK = false;
    }
  }
  
  //chegou request TELE?
  if (dados->STATUSTBOT) {
    if(dados->topicoTele.equals("/gas")){
      executarPublicar(dados->topicoTele, dados->subtopicoTele, dados->payloadTele);
      io->gasBot->postarTelegram(dados->payloadOUT);       
      io->dados->STATUSTBOT = false;
    }
  }
}

void SisIoTJJRDBGas::verificarVazamento(){
  if(mq5->verificarVazamento()) { executarPublicar("/gas","/dispararalerta","JJRupAlerta");  }
}

String SisIoTJJRDBGas::executarPublicar(String topicoIN, String subtopicoIN, String payloadIN ) {
  Serial.println("#### Gas-------------> ep(entrou topicoIN: "+topicoIN+" "+subtopicoIN+" "+payloadIN);   
  dados->topicoIN = topicoIN; 
  executarGas(subtopicoIN, payloadIN);//saida dados->payloadOUT
  
  io->publicarMQTT(dados->topicoOUT, dados->subtopicoOUT, dados->payloadOUT );
  Serial.println("#### Gas-------------> ep(Saiu) "+dados->topicoOUT+" "+dados->subtopicoOUT+" "+dados->payloadOUT);
  return dados->payloadOUT;
}

void SisIoTJJRDBGas::dashboardGas() { //vai exibindo no Front
  io->publicarMQTT("/gas", "/sirene"          , String(dados->_sirene));
  io->publicarMQTT("/gas", "/sensor"          , String(mq5->getSensorV()));
  io->publicarMQTT("/gas", "/limite"          , String(mq5->getLimiteV()));
  io->publicarMQTT("/gas", "/vazamento/ultimo", String(mq5->getVazamentoVUltimo()));
  io->publicarMQTT("/gas", "/vazamento/max"   , String(mq5->getVazamentoVMax()));
  io->publicarMQTT("/gas", "/cmd"            , "/");

  io->publicarMQTT("/gas", dados->_display, io->msgDinamica());
}

void SisIoTJJRDBGas::alimentar(String msgbot) {
  Serial.println("#### -------------> alimentar(Entrou)");
  Serial.println("#### ---> SENSOR: " + String(mq5->getSensorV()) + "V " + dh->getHora() + " " + dh->getDataBR());
    
  lerMensagens();//MQTT E BOT
  
  verificarVazamento();
  
  dashboardGas();

  //Default
  dados->payloadBot = "";  
}

String SisIoTJJRDBGas::executarGas(String subtopicoIN, String payloadIN) {
  dados->topicoOUT    = dados->topicoIN;
  dados->subtopicoOUT = subtopicoIN;//definir saida

  if (subtopicoIN.equals(dados->_display) || subtopicoIN.equals("/displaymsg")) { //qdo receber msg mqtt ou bot, exibir nos display
    dados->payloadOUT = payloadIN;

  } else if( subtopicoIN.equals("/start") || subtopicoIN.equals(dados->IDBOT) ) {
    dados->payloadOUT = "Bem-vindo ao SisIoT Joao Junior HOME :]\nMeu nome é " + dados->NOMEBOT +
                       ", sou o Adm do sistema. Por favor, digite / para exibir os comandos.";

  }else if(subtopicoIN.equals("/dispararalerta") ) {
    if(payloadIN.equals("JJRupAlerta")) { 
      dispararAlerta(); 
      dados->payloadOUT   = "Vazamento de "+String(mq5->getSensorV())+"V às "+dados->_horaVazUltimo;
    }else{ dados->payloadOUT="Nada para Atualizar!"; }
  
  }else if(subtopicoIN.equals("/data") ) {  
    dados->payloadOUT = dh->getDataBR();

  } else if(subtopicoIN.equals("/hora") ) { //payload=0 ou false
    dados->payloadOUT = dh->getHms(true, true, payloadIN.toInt()); //H:m

  } else if( subtopicoIN.equals("/sirene") ) { //Pode ser acionado via botao ou valor do sensorV
    if ( !payloadIN.equals("") ) {
      dados->_sirene = payloadIN.toInt();
      autom->acionarPino(PinGAS,dados->_sirene);
    }
    dados->payloadOUT = String(dados->_sirene);//0 ou 1

  } else if(subtopicoIN.equals("/limite") ) { //grafico
    if ( !payloadIN.equals("") ) {
      mq5->setLimiteV(payloadIN.toFloat());
    }
    dados->payloadOUT = String(mq5->getLimiteV());

  } else if(subtopicoIN.equals("/sensor") ) { //grafico
    if (!payloadIN.equals("")) {
      dados->_sensorV = payloadIN.toFloat();  //atribuicao direta para nao cair no calculo do V
    }    
    dados->payloadOUT = String(mq5->getSensorV());
    
  } else if(subtopicoIN.equals("/resetar") ) {
    Serial.print("Reiniciando...");
    autom->setResetar(true);//
  
  } else if(subtopicoIN.equals("/ip") ) {
    dados->subtopicoOUT = "/displaymsg"; dados->payloadOUT = tools->getIp();

  } else if( subtopicoIN.equals("/ajuda")) {
    dados->subtopicoOUT = "/displaymsg"; dados->payloadOUT = "Digite /CMD exibir os comandos!";

  } else if(subtopicoIN.equals("/link") ) {
    dados->subtopicoOUT = "/displaymsg"; dados->payloadOUT = dados->urlTS;

  } else if( subtopicoIN.equals("/cmd")) {
    dados->subtopicoOUT = "/displaymsg";//atualizar saida
    dados->payloadOUT = "/IP /ALERTA /SENSOR\n /LINK /AJUDA /LIMITE=5.5";

  } else { //Se nao eh comando vira msg para sera exibida no LCD.
    dados->payloadOUT = subtopicoIN;
    dados->payloadBot = subtopicoIN;
    dados->subtopicoOUT = "/displaymsg";//atualizar saida
  }
  return dados->payloadOUT;
}

void SisIoTJJRDBGas::updateDadosDeAlerta() {
  dados->subtopicoOUT = "/displaymsg";//atualizar saida
  //Dados do sensor ja foram atualizados em: mq5.verificarVazamento()
  dados->_sirene      = true;
  dh->setAnoDiaMes(dh->getData());                      //int
  dh->setHHmmss(dh->getHora());                         //int
  dados->_dataVazUltimo  = dh->getDataBR();
  dados->_horaVazUltimo  = dh->getHms(true, true, true);
  dados->payloadBot      = io->atualizarMsgAlerta();
  dados->payloadOUT      = dados->payloadBot;
  Serial.println("####### ALETAR UP payloadOUT: "+dados->payloadOUT);
}

void SisIoTJJRDBGas::dispararAlerta(){
  updateDadosDeAlerta();
    
  autom->acionarPino(PinGAS, dados->_sirene); //Buzzer ou LED ou Relé->Sistema-Anti-incêndio
  
  io->publicarMQTT(dados->topicoOUT,dados->subtopicoOUT, dados->payloadOUT);//msg alerta
  dashboardGas();// dados ja atualizados
  
  tools->testarConexao();
  io->postarThingSpeak(mq5->getSensorV());  
  tools->delay2(5000);//exigencia do TS

  io->postarTelegram(dados->payloadBot);     

  //Default
  dados->_sirene =false;
  autom->acionarPino(PinGAS, dados->_sirene);
  dados->statusVazamento = false;
}
