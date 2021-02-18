/*
  SisIoTJJRMQTT.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRMQTT_h//guarda de inclusão
#define SisIoTJJRMQTT_h

//Libs includes
#include "SisIoTJJRDados.h"
#include "SisIoTJJRTools.h"

class SisIoTJJRMQTT{
    private:
      //*****************************************
      const char*   _serverMQTT     = "broker.mqtt-dashboard.com";//broker.mqtt-dashboard.com
      const int     _portaMQTT      = 1883;
      bool          _mqttConectado  = false;          //      prefixo/top/acao/subtop/valor
      const char*   _prefixo        = "";             //
    public:      
      //APP Variaveis Publicas*******************  
      SisIoTJJRDados*        dados;
      SisIoTJJRTools         tools;

      PubSubClient*             pscmqtt;
      //Construtores*****************************
      SisIoTJJRMQTT(SisIoTJJRDados& _dados);
      
      //Metodos APP******************************  
      void    callback(char* topic, byte* payload, unsigned int length);
      void    processarTopSubPay(String topico, String payload);
      String  postarAlertaMQTT(String payload);
      void    publicarTopico(String topico,String subtopico,String resposta);
      void    inscreverTopico(String topico, String subtopico);      
      void    reconectarMQTT();
      void    verificarMQTT();
      void    configurarMQTT(); 
};

#endif
