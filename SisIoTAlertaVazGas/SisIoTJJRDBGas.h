/*
  SisIoTJJRDBGas.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRDBGas_h//guarda de inclusão
#define SisIoTJJRDBGas_h

//Libs includes
#include "SisIoTJJRDados.h"
#include "SisIoTJJRDataHora.h"
#include "SisIoTJJRAutomocao.h"
#include "SisIoTJJRSensorGas.h"
#include "SisIoTJJRTools.h"
#include "SisIoTJJRio.h"

//LimiteV
#define _LIMITE_V 5.01

class SisIoTJJRDBGas{
    private:
      //APP************************************** 
       
    public:      
      //APP Variaveis Publicas*******************  
      SisIoTJJRDados*            dados;
      SisIoTJJRDataHora*         dh;
      SisIoTJJRAutomocao*        autom;
      SisIoTJJRSensorGas*        mq5;
      SisIoTJJRTools*            tools;
      SisIoTJJRio*               io;
      
      //Construtores*****************************
      SisIoTJJRDBGas(SisIoTJJRDados&     _dados, SisIoTJJRDataHora&  _dh, SisIoTJJRAutomocao& _autom, 
                     SisIoTJJRSensorGas&   _mq5, SisIoTJJRTools&      _tools, SisIoTJJRio&        _io  );    
      //Metodos *********************************   
      void   lerMensagens();
      void   verificarVazamento();
      String executarPublicar(String topicoIN, String subtopicoIN, String payloadIN );
      void   dashboardGas();
      void   alimentar(String msgbot);
      String executarGas(String subtopicoIN, String payload);
      void   updateDadosDeAlerta();
      void   dispararAlerta();  ;
      
};

#endif
