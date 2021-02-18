/*
  SisIoTJJRio.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRio_h//guarda de inclusão
#define SisIoTJJRio_h

//Libs includes
//#include <Arduino.h>

#include "SisIoTJJRDados.h"
#include "SisIoTJJRTools.h"
#include "SisIoTJJRDataHora.h"
#include "SisIoTJJRSensorGas.h"
#include "SisIoTJJRMQTT.h"
#include "SisIoTJJRTelegram.h"//No UBUNTU Telegram - instalei a ultima versao ArduinoJson 6.17.2 via Gerenciador de biblhomeecas da Arduino IDE
                              //Telegram - instalar a versao ArduinoJson 6.15.2 via Gerenciador de biblhomeecas da Arduino IDE

//ThingSpeak
#define _API_KEY_TWEET "" // API key para ThingTweet
#define _APIKEY_TS "" // API key para ThingSpeak                              

class SisIoTJJRio{
    private:
      //*****ThingSpeak*******************************
      const String _apiKeyTS  = _APIKEY_TS; // API key para ThingSpeak     
      const char*  _serverTS  = "api.thingspeak.com";
      const int     _portaTS  = 80;
      //*******************************
    public:      
      //APP Variaveis Publicas*******************  
      
      SisIoTJJRDados*           dados;
      SisIoTJJRTools*            tools;  
      SisIoTJJRDataHora*         dh;
      SisIoTJJRSensorGas*        mq5;
      SisIoTJJRMQTT*             mqtt;   
      SisIoTJJRTelegram*         gasBot;
            
      //Construtores*****************************
      
      SisIoTJJRio( );
      SisIoTJJRio(SisIoTJJRDados& _dados, SisIoTJJRTools&  _tools, SisIoTJJRDataHora&   _dh, SisIoTJJRSensorGas&  _mq5, SisIoTJJRMQTT& _mqtt, SisIoTJJRTelegram& _gasBot );
      //*****************************************
      bool postarTelegram(String payOUT );
      void publicarMQTT(String topicoIN, String subtopicoIN, String payloadIN );   
      //*****ThingSpeak*******************************      
      void postarThingSpeak(float sensorVolt);  
      //*****MSG******************************* 
      String msgDinamica();
      String atualizarMsgAlerta();
};

#endif
