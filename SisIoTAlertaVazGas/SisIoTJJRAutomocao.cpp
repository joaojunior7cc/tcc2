#include "SisIoTJJRAutomocao.h"

SisIoTJJRAutomocao::SisIoTJJRAutomocao(){

}

SisIoTJJRAutomocao::SisIoTJJRAutomocao(SisIoTJJRDados& _dados){
  this->dados = &_dados;
}


void SisIoTJJRAutomocao::configurarPinos() {
  //pinMode(16, INPUT_PULLUP);// D0 sinal digital
  //pinMode(PinD1, OUTPUT);     //Pino D1 = 5  
  //pinMode(PinD2, OUTPUT);     //Pino D2 = 4 
  //pinMode(PinD3, OUTPUT);     //Pino D3 = 0
  pinMode(PinD4, OUTPUT);     //Pino D4 = 2  
  //pinMode(PinD5, OUTPUT);     //Pino D5 = 14 
  pinMode(PinGAS, OUTPUT); //Pino D6 = 12
  //pinMode(PinD7, OUTPUT); //Pino D7 = 13 
  //pinMode(PinD8, OUTPUT);     //Pino D8 = 15    
  //pinMode(PinD9, OUTPUT);     //Pino D9 = 3   
  //pinMode(PinD10, OUTPUT);     //Pino D10 = 1  
  
  acionarPino(PinD4, true);   //IR: apagarLED Azul do ESP
}

bool SisIoTJJRAutomocao::acionarPino(int pinoDx, boolean estadoPinoDx){
  if(estadoPinoDx==true){//Ligado
     digitalWrite(pinoDx,HIGH);         
     return true;
  }else{//Desligado
     digitalWrite(pinoDx,LOW);
     return false;
  }  
}

bool SisIoTJJRAutomocao::getResetar(){
  return dados->_resetar;
}

void SisIoTJJRAutomocao::setResetar(bool resetar){
  dados->_resetar = resetar;
}
