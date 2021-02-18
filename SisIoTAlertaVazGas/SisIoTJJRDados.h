/*
  SisIoTJJRDados.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRDados_h//guarda de inclusão
#define SisIoTJJRDados_h

//Libs includes
#include <Arduino.h>//Automocao
#include <string.h>

//Grafico(ThingSpeak) e MQTT
#include <WiFiClient.h>

//Data e Hora
#include "NTPClient.h"//Biblioteca do NTP.
#include <WiFiUdp.h>//Biblioteca do UDP.

//MQTT
//#include <WiFiClient.h>
#include "PubSubClient.h"

//NET
#include <ESP8266WiFi.h>

//SENSOR_GAS
#define _LIMITE_V 5.01

//NET
#define _SSID " "
#define _PASS " " 

//Pinos
//#define PinD0 16    //Pino D0 = 16  
//#define PinD1 5    //Pino D1 = 5  
//#define PinD2 4    //Pino D2 = 4 
//#define PinD3 0    //Pino D3 = 0
#define PinD4 2    //Pino D4 = 2  (LED AZUL DO ESP)
//#define PinD5 14   //Pino D5 = 14 
#define PinGAS 12   //Pino D6 = 12
//#define PinD7 13   //Pino D7 = 13
//#define PinD8  15   //Pino D8 = 15    
//#define PinD9  3    //Pino D9 = 3   
//#define PinD10 1    //Pino D10 = 1  

class SisIoTJJRDados{
    private:
    //APP*******************************************
    
    public:      
    //Construtores**********************************
      SisIoTJJRDados();

    //APP Variaveis Publicas************************   
    //*****GERAL**************************************
      WiFiClient client;//Grafico(ThingSpeak) e MQTT
      unsigned long _lastTime       = 0;
      unsigned long _mtbs           = 2000; //Sandro mean time between scan messages 2000 ou 1000()        

    //*****ThingSpeak*******************************
      const String urlTS   = "https://thingspeak.com/channels/xxxxxxxxx"; 
  
    //*****Automocao********************************
      bool _resetar        = false;  
      
    //*****Data e Hora******************************  
      int _ano                = -1;//armazanar qdo preciso
      int _mes                = -1;//
      int _dia                = -1;//
      int _h                  = -1;//
      int _m                  = -1;//
      int _s                  = -1;//armazanar qdo preciso      
      NTPClient*    ntp;
      String _data            = "0101-01-01";
      String _hora            = "00:00:00"; 
      String _dataVazUltimo   = "";
      String _horaVazUltimo   = "";  
      
    //*****MQTT**************************************  
      const String  _display        = "/display";//
      bool   STATUSCALLBACK         = false;//
      
      String topicoIN               = "/home";
      //String subtopicoIN            = "/displaymsg";
      //String payloadIN              = "Sistema rodando...";
      
      String topicoOUT              = "/home";
      String subtopicoOUT           = "/displaymsg";
      String payloadOUT             = "Sistema rodando...";
      
      String topicoCall             = "gas";
      String acaoCall               = "out";
      String subtopicoCall          = "/displaymsg";
      String payloadCall            = "Sistema rodando...";
            
      String        _topico         = "";
      const char*   _in             = "/in";
      const char*   _out            = "/out";
      String  _acao                 = "";
      String  _subtopico            = "";  
      String        _payloadDpy     = "";           // msg so para o /display
      bool          _alternarMsgVaz = true;         // serve para alternar as msg no /display no alimentar()

    //*****MSG***************************************      
      String payloadDpy                 = "";           // msg so para o /display   
      String msgAlerta                  = "[...]"; 
      bool   alternarMsgVaz             = true;// serve para alternar as msg no /display no alimentar()  
      const String _msgAlertaCabecalho  = "SisIoTGas MQ-5";  

    //*****NET***************************************     
      String ssid = _SSID;
      
    //*****SENSORGAS*********************************        
      float  statusVazamento    = false;
      float  _sensorV           = 0.0; //dinamicamento lido no loop() raiz
      float _vazamentoVUltimo   = 0.0;
      float _vazamentoVMax      = 0.0;
      float _limiteV            = _LIMITE_V;//Default 

    //*****Telegram**********************************     
      bool  STATUSTBOT  = false;//
      const String NOMEBOT      = "SeuBot";
      const String IDBOT        = "@SeuBot";
      String topicoTele         = "gas";
      String acaoTele           = "out";
      String subtopicoTele      = "/displaymsg";
      String payloadTele        = "Olá, meu nome é "+NOMEBOT;     
      String topicoPayload      = "";      
      String payloadBot         = "";               
      String _chat_id_lido      = "";

    //*****DB GAS**********************************     
      bool _sirene         = false;// GAS 
    

};

#endif
