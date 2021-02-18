#include "SisIoTJJRDataHora.h"

//DataHora
WiFiUDP udp;//Cria um objeto "UDP".
NTPClient _ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.
      
SisIoTJJRDataHora::SisIoTJJRDataHora(){
  dados->ntp = &_ntp;
}
SisIoTJJRDataHora::SisIoTJJRDataHora(SisIoTJJRDados& _dados){
  this->dados = &_dados;
  dados->ntp  = &_ntp;
}

void SisIoTJJRDataHora::atualizarDataHora(){
  dados->ntp->begin();
  int tentativas=0;  
  while(!dados->ntp->update()&&tentativas<7) {
    dados->ntp->forceUpdate();
    Serial.print(".");
    tentativas=tentativas+1;
    delay(50);
  }   
  setHora(dados->ntp->getFormattedTime());//HORA atual
  String dataTemp = dados->ntp->getFormattedDate();//data
  int i           = dataTemp.indexOf("T");
  setDATA( dataTemp.substring(0, i) );//data cortada
  dados->ntp->end();
}

String SisIoTJJRDataHora::getData(){
  return dados->_data;
}

void SisIoTJJRDataHora::setDATA(String data){
  dados->_data = data;
}

String SisIoTJJRDataHora::getHora(){
  return dados->_hora;
}

void SisIoTJJRDataHora::setHora(String hora){
  dados->_hora = hora;
}

void SisIoTJJRDataHora::setAnoDiaMes(String data){
  String ano, mes, dia;
  ano = data.substring(0,4);       // 2020-12-26
  dados->_ano = ano.toInt();
  mes = data.substring(5,7);
  dados->_mes = mes.toInt();
  dia = data.substring(8);    
  dados->_dia = dia.toInt();  
}

void SisIoTJJRDataHora::setHHmmss(String hora){
  String aux;  
  //Guardando H m s (int):       00:22:11
  aux  = hora.substring(0,2);//00  
  dados->_h = aux.toInt();
  aux = hora.substring(3,5);
  dados->_m   = aux.toInt();
  aux = hora.substring(6);
  dados->_s   = aux.toInt();
}

String SisIoTJJRDataHora::getDataBR(){
  String ano, mes, dia;
  ano = dados->_data.substring(0,4);       // 2020-12-26
  mes = dados->_data.substring(5,7);
  dia = dados->_data.substring(8);    
  return dia+"/"+mes+"/"+ano;
}

String SisIoTJJRDataHora::getHms(bool h,bool m, bool s){
  String hrTemp = dados->_hora;//00:11:22
  String aux;
  
  if(!m && !s){//h
    return dados->_hora.substring(0,2); 
  }else if(!s){//h:m
    return dados->_hora.substring(0,5);
  }    
  return dados->_hora;//h:m:s  
}
