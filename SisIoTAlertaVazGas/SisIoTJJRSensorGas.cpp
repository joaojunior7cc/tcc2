#include "SisIoTJJRSensorGas.h"
      
SisIoTJJRSensorGas::SisIoTJJRSensorGas(SisIoTJJRDados& _dados ){
  this->dados = &_dados;
}

float SisIoTJJRSensorGas::getVazamentoVMax(){
  return dados->_vazamentoVMax;
}

void SisIoTJJRSensorGas::setVazamentoVMax(float vazamento){
  if( vazamento > dados->_vazamentoVMax ){ dados->_vazamentoVMax = vazamento; }
} 

float SisIoTJJRSensorGas::getVazamentoVUltimo(){
  return dados->_vazamentoVUltimo;
}

void SisIoTJJRSensorGas::setVazamentoVUltimo(float vazamento){
  dados->_vazamentoVUltimo = vazamento;
}

float SisIoTJJRSensorGas::getLimiteV(){
  return dados->_limiteV;
}
  
void SisIoTJJRSensorGas::setLimiteV(float limiteV){
  if( limiteV>0 ){ dados->_limiteV = limiteV; }//se positivo, eu mudo
}

float SisIoTJJRSensorGas::getSensorV(){
  return dados->_sensorV;
}

void SisIoTJJRSensorGas::setSensorV(float adc){       //atribui se value nao for NaN
  if(!isnan(adc)){ dados->_sensorV = adc*(5.0/1023); }  //calculo V
}

bool SisIoTJJRSensorGas::verificarVazamento(){
  if( getSensorV() > getLimiteV() ){//vazamento
    dados->statusVazamento  = true;
    setVazamentoVMax(getSensorV());
    setVazamentoVUltimo(getSensorV());//grafico
    
    return dados->statusVazamento;
  }else{
    return false;
  }
}
