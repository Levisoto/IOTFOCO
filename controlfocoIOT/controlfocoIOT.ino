#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define LED D0            // Led in NodeMCU at pin GPIO16 (D0).

ESP8266WiFiMulti wifiMulti;
boolean connectioWasAlive = true;
boolean connection = false;
int n;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  /*wifiMulti.addAP("Laxus", "illdoit1");
  wifiMulti.addAP("MOVISTAR_0BB0", "Kgbn4EkKtsJdbGCfx7XD");
  wifiMulti.addAP("iPhone de Gfjf", "12345678");
  wifiMulti.addAP("LG K8 (2017)", "12345678");*/

  /*
  Serial.print("Connecting Wifi ...");
  if(wifiMulti.run() != WL_CONNECTED ){
    Serial.println("");
    Serial.println("Wifi Connected");
    Serial.println("Ip Adress:");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    }
    */

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi...");

    while(wifiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }

    if(wifiMulti.run() == WL_CONNECTED ){
      connectioWasAlive = false;
      Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
      }
  int n=0;
}



void loop() {
  // put your main code here, to run repeatedly:
 //monitorWiFi();
if(wifiMulti.run() != WL_CONNECTED ){
  
    Serial.print(".");
    if (connectioWasAlive == true)
    {
      connectioWasAlive = false;
      Serial.print("Looking for WiFi.");
    }
    Serial.print(".");
    delay(500);
  }
 else{
  
  if (connectioWasAlive == false || connection == true)
  {
    
   if(connection == false || connectioWasAlive == false){
      connection = true;
      Serial.printf(" connected to %s\n", WiFi.SSID().c_str());
      if(Firebase.failed()){
        Serial.print("restarting.");
        ESP.restart();
        delay(100);
        }
      
      
    }else{
      n=Firebase.getInt("LIGHT_STATUS");
      Serial.println(n);
      switch (n){
         case 1:  {digitalWrite(D0,LOW);  break;}
         case 0: {digitalWrite(D0,HIGH); break;}
      }
        delay(500);
      }
      connectioWasAlive = true;
  }
  
  } 
}

/*
else{
  n=Firebase.getInt("LIGHT_STATUS");
  Serial.print(n);

 switch (n){
  case 1:  {digitalWrite(D0,LOW);  break;}
  case 0: {digitalWrite(D0,HIGH); break;}
  }
  delay(500);
  }*/
