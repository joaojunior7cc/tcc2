/*
  SisIoTJJRTools.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRTools_h//guarda de inclusão
#define SisIoTJJRTools_h

//Libs includes
#include <Arduino.h>
#include "SisIoTJJRDados.h"

class SisIoTJJRTools{
    private:
      //*****************************************
      
      //*****************************************
      
    public:      
      //APP Variaveis Publicas*******************  
      SisIoTJJRDados*   dados;   
      //Construtores*****************************
      SisIoTJJRTools();
      SisIoTJJRTools(SisIoTJJRDados& _dados);
      //*****************************************
      void    delay2(int miliseg);
      String  cutChar(String s, char c, int prePos);      
      //***NET*************************************
      void conectarWifi( );
      bool testarConexao();
      String getIp();
};

#endif
