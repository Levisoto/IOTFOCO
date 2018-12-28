#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST ""
#define FIREBASE_AUTH "" 
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define LED D0            // Led in NodeMCU at pin GPIO16 (D0).
#define LED D1            

int suma; // creat this variable to work without the lost of connection

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

}

int n=0;
int sum=0;
void loop() {

suma=suma+1; // This variable working always
if (suma==20){
  suma=0;
}
Firebase.set("sum",suma); // Send the data of suma all the time, so the connection nevel will end 
n=Firebase.getInt("LIGHT_STATUS");


    if (n==1) {
    digitalWrite(D0,LOW); 
    digitalWrite(D1,HIGH);
    delay(10);
}

if (n==0) {
    digitalWrite(D0,HIGH); 
    digitalWrite(D1,LOW);
    delay(10);

}
}
