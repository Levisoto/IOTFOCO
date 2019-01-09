#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include<FirebaseArduino.h>
#define FIREBASE_HOST "uvas-studio.firebaseio.com"
#define FIREBASE_AUTH "99f1tP31pKnda4ORx4Ba4gtuO8Uda9Epj2HrjqDr"  

char* WIFI_SSID = "Laxus";
char* WIFI_PASSWORD = "illdoit1";
#define LED D0  // Led in NodeMCU at pin GPIO16 (D0).


const char* host = "level-studio.herokuapp.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "08 3B 71 72 02 43 6E CA ED 42 86 93 BA 7E DF 81 C4 BC 62 30";


ESP8266WiFiMulti wifiMulti;
boolean connectioWasAlive = true;
boolean connection = false;
int n;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(LED, OUTPUT); 

    Serial.println();
    Serial.print("Wait for WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  
  Serial.println("Connected");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  connectioWasAlive = false;
}

void loop() {
WiFiClientSecure client;

String WIFI_SSID2 = Firebase.getString("SSID");  
String WIFI_PASSWORD2 =Firebase.getString("PASSWORD");  


  while (WiFi.status() != WL_CONNECTED || WiFi.localIP() == IPAddress(192,168,1,1)) {
    
    if (connectioWasAlive == true)
    {
      connectioWasAlive = false;
     
      WiFi.begin(WIFI_SSID2, WIFI_PASSWORD2);
      Serial.print("Looking for WiFi.");
    }
    Serial.print(".");
    delay(500);
  }

  if (connectioWasAlive == false || connection == true)
  {
   if(connection == false || connectioWasAlive == false){
      connection = true;
      Serial.printf(" connected to %s\n", WiFi.SSID().c_str());     
      Serial.println("true");
    }else{
      
      Serial.print("connecting to ");
      Serial.println(host);
      if (client.connect(host,httpsPort)) {
        if (client.verify(fingerprint, host)) {
            Serial.println("certificate matches");
            } else {
            Serial.println("certificate doesn't match");
        }
          Serial.println("connected]");
          String url = "/api/lightStatus";
          Serial.println("[Sending a request]");
          client.print(String("GET /") + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n" +
                 "\r\n"
                );
         }
         else { Serial.println("Error with server"); return;}
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
         case 0:  {digitalWrite(D0,LOW);  Serial.print("LOW");break;}
         case 1: {digitalWrite(D0,HIGH); Serial.print("HIGHT");break;}
        }

      }

      // Close the connection
      Serial.println();
      Serial.println("closing connection");
      client.stop();

      delay(1000); // execute once every 5 minutes, don't flood remote service

      }
      connectioWasAlive = true;
    }
    
}
