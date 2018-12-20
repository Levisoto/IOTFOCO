#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define LED D0            // Led in NodeMCU at pin GPIO16 (D0).

float suma;
void setup() {
  // put your setup code here, to run once:

  pinMode(LED, OUTPUT);    // LED pin as output.
  Serial.begin(9600);
 WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
     delay(400);
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.set("LIGHT_STATUS",0);
}

int n=0;

void loop() {
  // put your main code here, to run repeatedly:
//suma=suma+1;
n=Firebase.getInt("LIGHT_STATUS");


    if (n==1) {
    digitalWrite(D0,LOW); 
    delay(10);
}

if (n==0) {
    digitalWrite(D0,HIGH); 
    delay(10);

}
}
