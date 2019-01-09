#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include<FirebaseArduino.h>
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""  
#define MY_SIZE 2

#define LED D6  // Led in NodeMCU at pin GPIO16 (D0).

const char* host = "level-studio.herokuapp.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "08 3B 71 72 02 43 6E CA ED 42 86 93 BA 7E DF 81 C4 BC 62 30";

String NAME;
String PASS; 
String ssdi[2];
String pass[2];


int analogPin = A0;   // potentiometer connected to analog pin
int val = 0;  

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
    WiFi.begin("Laxus","illdoit1");

    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    if(WiFi.status() == WL_CONNECTED) {
        Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
        while(NAME==""){
        NAME = Firebase.getString("SSID");  
        PASS =Firebase.getString("PASSWORD"); 
        delay(500);
        }
    WiFi.disconnect();

    ssdi[0]="Laxus";
    pass[0]="illdoit1";
    ssdi[1]=NAME;
    pass[1]=PASS;
    
    }
}


void conectToWifi(String ssdi[], String pass[]){
  for(int i=0; i< MY_SIZE && (WiFi.status() != WL_CONNECTED); i++){
     
     if(WiFi.status() == WL_CONNECTED) {Serial.println("Conected");}
     else{
      WiFi.begin(ssdi[i].c_str(),pass[i].c_str());
     Serial.println("Connecting" + ssdi[i]+".");
       for(int j=0; j< 15 && (WiFi.status() != WL_CONNECTED); j++){
         Serial.print(".");
         delay(500);
      }
      Serial.println("");

     }
      
  } 
}

void loop() {
WiFiClientSecure client;


if(WiFi.status() != WL_CONNECTED ){
  conectToWifi(ssdi,pass);
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
         case 0:  {digitalWrite(LED,LOW);  Serial.print("LOW");break;}
         case 1: {val = analogRead(analogPin);analogWrite(LED, val / 4); Serial.print("HIGHT");break;}
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
}
