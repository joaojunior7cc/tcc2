#include "SisIoTJJRTools.h"

SisIoTJJRTools::SisIoTJJRTools(){
}

SisIoTJJRTools::SisIoTJJRTools(SisIoTJJRDados& _dados){
  this->dados = &_dados;
}

void SisIoTJJRTools::delay2(int miliseg) {
  delay(miliseg);
  yield();//Efetua o feed do SW WDT.
  ESP.wdtFeed();//Alimenta o Watchdog.
}

String SisIoTJJRTools::cutChar(String s, char c, int prePos) {
  int iC = s.indexOf(c);//           iC=/
  if (prePos == 0) { //                   pre/pos
    return s.substring(0, iC);//pre
  } else if (prePos == 1) { //1
    return s.substring(iC + 1); //       pos
  } else {
    return s;
  }
}

/***NET****/
void SisIoTJJRTools::conectarWifi( ){  
  Serial.println("conectarWifi(Entrou)");
  WiFi.begin(_SSID, _PASS); 
  while(WiFi.status() != WL_CONNECTED) {    
    Serial.print(".");
    delay(500);    
  }    
  Serial.println("conectarWifi(Saiu)");
} 

bool SisIoTJJRTools::testarConexao(){  
  if(WiFi.status() != WL_CONNECTED){
    return false;
  }else{
    return true;
  }
}

String SisIoTJJRTools::getIp(){  
  IPAddress ip = WiFi.localIP();  
  return ip.toString();
}
