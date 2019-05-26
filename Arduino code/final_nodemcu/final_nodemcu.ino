#include <Servo.h>

#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <Keypad.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <ArduinoJson.h>

// Set these to run example.
#define FIREBASE_HOST "smartdeliverybox-5dc37.firebaseio.com"
#define FIREBASE_AUTH "YCNHSgveJ85Oft0KpNtkRSrECc64iknah365uIvs"

#define WIFI_SSID "tt"  
#define WIFI_PASSWORD "12345678"

void setup(){
        Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //Serial.print(FIREBASE_HOST);
   Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
      delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    if (Firebase.failed()) { 
     Serial.print("setting /number failed:"); 
     Serial.println(Firebase.error());   
     return; 
 } 
    Firebase.setInt("LEDStatus",0);
}
 
void loop(){
  Serial.println("loop");
      Serial.println(Firebase.getInt("number"));
      Firebase.setInt("LEDStatus",1);
}
