#include "SisIoTJJRMQTT.h"

//MQTT
WiFiClient clientMQTT;//thingspeak e mqtt
PubSubClient _pscmqtt(clientMQTT);

SisIoTJJRMQTT::SisIoTJJRMQTT(SisIoTJJRDados& _dados){  
  this->dados = &_dados;
  pscmqtt     = &_pscmqtt;
}

void SisIoTJJRMQTT::callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("####################-------------------> Callback(Entrou)");
  Serial.print("###########----------->Mensagem que chegou [");
  Serial.print(topic);
  Serial.print("] ");
  
  //lendo msg(payload)
  dados->payloadCall ="";    
  for (int i = 0; i < length; i++) {
    dados->payloadCall += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println("");//                                           prefico/topico/acao/subtopico 

  processarTopSubPay(topic,dados->payloadCall);

  dados->STATUSCALLBACK = true;
  tools.delay2(100);
  Serial.println("####################-------------------> Callback(Saiu)");
}

void SisIoTJJRMQTT::processarTopSubPay(String topico, String payload) {
  //Serparar                                                   SisIotJJRHome/gas/in/sensor
  String topAcaoSub    = tools.cutChar(topico,'/',1);                      //       gas/in/sensor
  dados->topicoCall    = "/" + tools.cutChar(topAcaoSub,'/',0);           //                     /gas   
  String acaoSub       = tools.cutChar(topAcaoSub,'/',1);                 //       in/sensor
  dados->acaoCall      = "/" + tools.cutChar(acaoSub,'/',0);              //                     /in
  dados->subtopicoCall = "/" + tools.cutChar(acaoSub,'/',1);              //                     /limite    

  if(dados->subtopicoCall.equals("/cmd")){// /sensor=1.22
      int i = payload.indexOf('=');
      if(i != -1){// tem '='
        dados->subtopicoCall = payload.substring(0,i);// /sensor
        dados->payloadCall   = payload.substring(i+1);// 1.22 
      }else{
        dados->subtopicoCall = payload;
        dados->payloadCall = "";
      }
  }
  
  //nao passa /cmd
  Serial.println("---> _topicoCall    :"+dados->topicoCall); 
  Serial.println("---> _acaoCall      :"+dados->acaoCall); 
  Serial.println("---> _subtopicoCall :"+dados->subtopicoCall); 
  Serial.println("---> _payloadCall   :"+dados->payloadCall);
}

//concatena mais facilmente const char*(topico) com String(subtopico)
void SisIoTJJRMQTT::publicarTopico(String topico,String subtopico,String resposta){//   /gas/sensor
  String _topico = this->_prefixo;
  _topico.concat(topico);
  _topico.concat(dados->_out);
  _topico.concat(subtopico);
  _topico.trim();
  const char* outTopico = _topico.c_str();
  const char* resp = resposta.c_str();
  verificarMQTT();
  pscmqtt->publish(outTopico,resp);//estou ouvindo este topico
}

//concatena mais facilmente const char*(topico) com String(subtopico)
void SisIoTJJRMQTT::inscreverTopico(String topico, String subtopico){
  String _topico = this->_prefixo;
  _topico.concat(topico);
  _topico.concat(dados->_in);
  
  if(!subtopico.equals("")){
    _topico.concat(subtopico);//raiz:  prefixo/gas/in/subtopico
  }//else: raiz:  prefixo/gas/in
  
  _topico.trim();
  const char* inTopico = _topico.c_str();
  pscmqtt->subscribe(inTopico);//estou ouvindo este topico
}

void SisIoTJJRMQTT::reconectarMQTT(){   
 Serial.print("Tentando conexão MQTT"); 
  while (!pscmqtt->connected()) {
    Serial.print(".");
    String clientId = "SisIoTJJRMQTT-";//criar um ID random
    clientId += String(random(0xffff), HEX);

    if (pscmqtt->connect(clientId.c_str())) {//tentar conectarip
      Serial.println("conectado!");
      this->_mqttConectado = true;//sem uso por enquanto      
      //pscmqtt->unsubscribe(dados->_inTopico);//desincrever este topico            
      inscreverTopico("/home","");//raiz:  prefixo/home/in
      inscreverTopico("/home","/cmd");   
      inscreverTopico("/home","/resetar");  
      inscreverTopico("/gas",""); //raiz:  prefixo/gas/in
      inscreverTopico("/gas","/cmd");   
      inscreverTopico("/gas","/sirene");
      inscreverTopico("/gas","/sensor");
      inscreverTopico("/gas","/limite");
      inscreverTopico("/fan",""); //raiz:  prefixo/fan/in
      inscreverTopico("/fan","/cmd");  
      
    } else {
      Serial.print("falhou, rc=");
      Serial.print(pscmqtt->state());
      Serial.println(" tente novamente em 5 segundos");      
      // esperar 5 segundos para tentar novamente
      this->_mqttConectado = false;      
    }
  }
}

void SisIoTJJRMQTT::verificarMQTT(){
  if (!pscmqtt->connected()) {
    reconectarMQTT();
  }
  pscmqtt->loop();
}

void SisIoTJJRMQTT::configurarMQTT(){
  pscmqtt->setServer(this->_serverMQTT, this->_portaMQTT);
  //pscmqtt->setCallback(dados->callback);//erro em lib personalizada (Funciona se fosse chamada la no Setup())
  pscmqtt->setCallback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
  
}
