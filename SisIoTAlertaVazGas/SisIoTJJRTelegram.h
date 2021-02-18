/*
  SisIoTJJRTelegram.h - Library for use with NodeMCU ESP8266 driver board
  xD License
*/

#ifndef SisIoTJJRTelegram_h//guarda de inclusão
#define SisIoTJJRTelegram_h

//Libs includes
#include "SisIoTJJRDados.h"

//telegrambot
#include "UniversalTelegramBot.h"//ArduinoJson 6.5.2 https://randomnerdtutorials.com/telegram-group-esp32-esp8266/
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>


//Telegram - //instalar a versao ArduinoJson 6.5.2 via Gerenciador de bibliotecas da Arduino IDE
#define _BOT_TOKEN "" //Define o Token do *seu* BOT
//Telegram ChatID
// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you

#define CHATID_GAS ""
//https://api.telegram.org/bot SEUTOKEN /getUpdates


class SisIoTJJRTelegram{
    private:
      //**************************************
      WiFiClientSecure*     _clientTelegram;
      UniversalTelegramBot* _bot;
      
    public:      
      //Variaveis Publicas*******************     
      SisIoTJJRDados*       dados;
      //Construtores*****************************
      SisIoTJJRTelegram();
      SisIoTJJRTelegram(SisIoTJJRDados& _dados);
      //Metodos APP******************************
      void certificado();
      //Telegram*********************************
      void tempoRecuperacao();
      void delay2(int miliseg);
      bool testarFuncao(String funcao);
      bool setComandos();
      bool enviarTelegram(const char* id, String msg);
      bool enviarAcaoBot(const char* id, String cmd);
      bool postarTelegram(String msg);
      void processarTopSubPay(String topPay);
      void lerTelegram();

      
};

#endif
