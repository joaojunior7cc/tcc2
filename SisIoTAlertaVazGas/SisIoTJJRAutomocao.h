/*
  SisIoTJJRAutomocao.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRAutomocao_h//guarda de inclusão
#define SisIoTJJRAutomocao_h

//Libs 
#include "SisIoTJJRDados.h"

class SisIoTJJRAutomocao{
    private:
      //*****************************************
 
      SisIoTJJRDados* dados;
    public: 
      //Construtores*****************************
      SisIoTJJRAutomocao();
      SisIoTJJRAutomocao(SisIoTJJRDados& _dados);
      //*******************************   
      void configurarPinos(); 
      bool acionarPino(int pinoDx, boolean estadoPinoDx);  
      bool getResetar();
      void setResetar(bool resetar);
      
};

#endif
