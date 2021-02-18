/*
  SisIoTJJRSensorGas.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRSensorGas_h//guarda de inclusão
#define SisIoTJJRSensorGas_h

#include "SisIoTJJRDados.h"

class SisIoTJJRSensorGas{
    private:
      //APP************************************** 
    public:      
      //APP Variaveis Publicas*******************        
      SisIoTJJRDados*    dados;
      
      //Construtores*****************************
      SisIoTJJRSensorGas(SisIoTJJRDados& _dados);      
      //Metodos APP******************************     
      float  getVazamentoVMax(); 
      void   setVazamentoVMax(float vazamento); 
      float  getVazamentoVUltimo(); 
      void   setVazamentoVUltimo(float vazamento); 
      float  getLimiteV();
      void   setLimiteV(float limiteV);
      float  getSensorV();
      void   setSensorV(float adc);
      bool   verificarVazamento();

      
};

#endif
