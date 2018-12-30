#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define FIREBASE_HOST "uvas-studio.firebaseio.com"
#define FIREBASE_AUTH "99f1tP31pKnda4ORx4Ba4gtuO8Uda9Epj2HrjqDr"
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
int LED = 2;            // Led in NodeMCU at pin GPIO16 (D0).

const char* host = "192.168.43.216";



ESP8266WiFiMulti wifiMulti;
boolean connectioWasAlive = true;
boolean connection = false;
int n;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(LED, OUTPUT); 
  /*wifiMulti.addAP("Laxus", "illdoit1");
  wifiMulti.addAP("MOVISTAR_0BB0", "Kgbn4EkKtsJdbGCfx7XD");*/
  wifiMulti.addAP("LG K8 (2017)", "12345678");
  
  //wifiMulti.addAP("iPhone de Gfjf", "12345678");

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
      }

    
}



void loop() {
WiFiClient client;
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
      Serial.println("true");
    }else{

      Serial.print("connecting to ");
      Serial.println(host);
      if (client.connect(host, 3000)) {
          Serial.println("connected]");

          Serial.println("[Sending a request]");
          client.print(String("GET /") + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );
         }
         
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          delay(60000);
          return;
        }
      }

      // Read all the lines of the reply from server and print them to Serial
      Serial.println("receiving from remote server");
      // not testing 'client.connected()' since we do not need to send data here
      while (client.available()) {
        char endOfHeaders[] = "\r\n\r\n";
        client.find(endOfHeaders);

        StaticJsonBuffer<100> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(client);
        //root.prettyPrintTo(Serial);

        int TPS = root["lightStatus"];
        Serial.println(TPS);

        switch (TPS){
         case 1:  {digitalWrite(LED,LOW);  Serial.print("LOW");break;}
         case 0: {digitalWrite(LED,HIGH); Serial.print("HIGHT");break;}
        }

      }

      // Close the connection
      Serial.println();
      Serial.println("closing connection");
      client.stop();

      delay(1000); // execute once every 5 minutes, don't flood remote service
      
   

       /*JsonObject& root = jsonBuffer.parseObject(line);
          int TPS = root["lightStatus"];
          Serial.println(TPS);*/
     
        /*switch (payload){
         case 1:  {digitalWrite(2,LOW);  break;}
         case 0: {digitalWrite(2,HIGH); break;}
        }*/
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
