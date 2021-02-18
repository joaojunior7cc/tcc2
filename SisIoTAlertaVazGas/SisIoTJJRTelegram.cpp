#include "SisIoTJJRTelegram.h"

//SisIoTAlertaVazGas: Telegram - //instalar a versao ArduinoJson 6.15.2 via Gerenciador de bibliotecas da Arduino IDE
WiFiClientSecure _clientTele;
UniversalTelegramBot _bott(_BOT_TOKEN, _clientTele);
X509List cert(TELEGRAM_CERTIFICATE_ROOT);//Necessario na nova versao da lib

SisIoTJJRTelegram::SisIoTJJRTelegram(){
  this->_clientTelegram = &_clientTele;
  this->_bot = &_bott;  
}      

SisIoTJJRTelegram::SisIoTJJRTelegram(SisIoTJJRDados& _dados){
  this->_clientTelegram = &_clientTele;
  this->_bot  = &_bott;  
  this->dados = &_dados;
}

void SisIoTJJRTelegram::certificado(){
  this->_clientTelegram->setTrustAnchors(&cert); // Add root certificate for api.telegram.org
}

void SisIoTJJRTelegram::delay2(int miliseg){  
  delay(miliseg);
  yield();//Efetua o feed do SW WDT.
  ESP.wdtFeed();//Alimenta o Watchdog.  
}

//Telegram ****************************************
void SisIoTJJRTelegram::tempoRecuperacao(){
  //Tempo padrao de espera da lib (UniversalTelegramBot) necessario para o Telegram funcionar
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t agora = time(nullptr);
  Serial.print("Tempo de recuperação (agora): "+ String(agora)+"\n");
  
  while (agora < 24 * 3600){// 24 * 3600 = 86400 seg
    Serial.print(".");
    delay2(100);
    agora = time(nullptr);//atualizando o time
  }
  Serial.println(agora);
}

//Definindos quais comandos o bot ira exibir no Telegram
bool SisIoTJJRTelegram::setComandos(){
  const String comandos = F("["
                            "{\"command\":\"ajuda\",  \"description\":\"Precisando de ajuda?\"},"
                            "{\"command\":\"cmd\", \"description\":\"Todos os comandos!\"},"
                            "{\"command\":\"link\",\"description\":\"Link do gráfico de vazamentos\"},"
                            "{\"command\":\"ip\",\"description\":\"Local do SisIoTAlertaVazGas\"},"
                            "{\"command\":\"sensor\",\"description\":\"Valor do lido (sensor=numero)\"},"
                            "{\"command\":\"limite\",\"description\":\"Limite Vazam. (limite=numero)\"},"
                            "{\"command\":\"data\",\"description\":\"Data\"},"
                            "{\"command\":\"hora\",\"description\":\"Hora\"}"
                            "]");
  Serial.println("### setComandos: "+String(this->_bot->setMyCommands(comandos)) );  
}

bool SisIoTJJRTelegram::enviarAcaoBot(const char* id, String cmd){
  //const char* cid = id.c_str();
  /*typing            for text messages    
    upload_photo      for photos    
    upload_video      for videos    
    record_video      for video recording
    upload_audio      for audio files    
    record_audio      for audio file recording    
    upload_document   for general files    
    find_location     for location data    
    upload_video_note for video notes    
    record_video_note for video note recording*/
  this->_bot->sendChatAction(id,cmd);//"typing"
}

bool SisIoTJJRTelegram::enviarTelegram(const char* id,String msg){
  Serial.println("enviarTelegram(Entrou)");

  if (this->_bot->sendMessage(id, msg, "")){//"Markdown"
    Serial.println("Enviado para o Telegram sucesso");
    dados->_chat_id_lido = CHATID_GAS;//janela padrao
    return true;
  }else{  
    Serial.println("Erro Telegram");  
    return false;
  }
  Serial.println("enviarTelegram(Saiu)");  
}

bool SisIoTJJRTelegram::postarTelegram(String msg) {  
  Serial.println("Post Telegram...");
  const char* cid = dados->_chat_id_lido.c_str();
  enviarAcaoBot(cid,"typing");//digitando
  delay2(100);
  return enviarTelegram(cid, msg);
}

//Modelo de saida: topico=/gas  subtopico=/limite  payload=7.33
void SisIoTJJRTelegram::processarTopSubPay(String topPay) {             //          caso:  (1) /limite=7.33
  Serial.println("#####------> processarTopSubPay(Entrou) ");           //                 (2) /limite@SisGasBot
                                                                        //                 (3) @SisGasBot
  int b1            = topPay.indexOf('/');// 1a. /
  int i             = topPay.indexOf('=');// 
  int a             = topPay.indexOf('@');// 
  Serial.println("##---> b1/ "+String(b1)+" i = "+String(i)+" a @ "+String(a));    
  if(topPay.equals(dados->IDBOT)){//@SisGasBot
    dados->topicoTele     = "/gas";
    dados->acaoTele       = "/in";
    dados->subtopicoTele  = topPay;
    dados->payloadTele    = "";
    
  }else {//if(dados->_chat_id_lido == CHATID_GAS){ 
    if(b1 != -1){// comando valido
      dados->STATUSTBOT = true;
      dados->topicoTele = "/gas";
      dados->acaoTele   = "/in";
      if(i != -1){//tem =        
        dados->subtopicoTele  = topPay.substring(0,i);//             /limite
        dados->payloadTele    = topPay.substring(i+1,a);//            7.33
      }else{//                                        /limite
        dados->subtopicoTele  = topPay;                //             /limite
        dados->payloadTele    = "";                                   // ""
      }
    }    
  }   
  Serial.println("---> _topicoTele    :"+dados->topicoTele); 
  Serial.println("---> _acaoTele      :"+dados->acaoTele); 
  Serial.println("---> _subtopicoTele :"+dados->subtopicoTele); 
  Serial.println("---> _payloadTele   :"+dados->payloadTele);
}
  
void SisIoTJJRTelegram::lerTelegram(){    
    Serial.println("####################-------------------> lerTelegram(Entrou)");
    unsigned long agora  = millis();      
    dados->_chat_id_lido = CHATID_GAS;//janela padrao
    dados->topicoPayload = "";
    
    if (agora > dados->_lastTime + dados->_mtbs)  {
      dados->_lastTime = agora;
      int numMsg = this->_bot->getUpdates(this->_bot->last_message_received + 1);

      for(int i=0; i<numMsg; i++){
        dados->_chat_id_lido =  this->_bot->messages[i].chat_id;  
        const char* cid = dados->_chat_id_lido.c_str();//String to const char*
        Serial.println("### id-> "+dados->_chat_id_lido);
        
        dados->topicoPayload = this->_bot->messages[i].text;//   /ip  ou  /limite=2.53
        Serial.println("### text-> "+dados->topicoPayload);
        
        if(!dados->topicoPayload.equals("")){ 
          processarTopSubPay(dados->topicoPayload);
          break;
        }//pego a primeira msg e saio...
      }
    }   
    Serial.println("####################-------------------> lerTelegram(Saiu)");
}
