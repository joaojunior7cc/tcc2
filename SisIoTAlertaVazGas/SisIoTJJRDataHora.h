/*
  SisIoTJJRDataHora.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRDataHora_h//guarda de inclusão
#define SisIoTJJRDataHora_h

//includes
#include "SisIoTJJRDados.h"

class SisIoTJJRDataHora{
    private:
      //APP**************************************
    
    public:      
      //APP Variaveis Publicas*******************
      SisIoTJJRDados*   dados;
      //Construtores*****************************
      SisIoTJJRDataHora();
      SisIoTJJRDataHora(SisIoTJJRDados& _dados);
      //Metodos APP******************************   
      void   atualizarDataHora();
      String getData();
      void   setDATA(String data);
      String getHora();
      void   setHora(String hora);
      String getDataBR();
      String getHms(bool h,bool m, bool s);
      String getIp();
      void   setAnoDiaMes(String data);
      void   setHHmmss(String hora);
      
};

#endif
