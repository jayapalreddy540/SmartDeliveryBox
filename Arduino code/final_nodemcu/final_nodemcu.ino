#include <Servo.h>

#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <Keypad.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <ArduinoJson.h>

// Set these to run example.
#define FIREBASE_HOST "smartdeliverybox-5dc37.firebaseio.com"
//https://smartdeliverybox-5dc37.firebaseio.com/"
#define FIREBASE_AUTH "IUiaENvs35uEHojYuuVpCP298HeAd9y5OVGhaDx5"
//Change line with your WiFi router name and password
//#define WIFI_SSID "IdeationZone"  
//#define WIFI_PASSWORD "srix@123"

#define WIFI_SSID "srija"  
#define WIFI_PASSWORD "srijareddy"

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D1, D2);  //D2=SDK  D1=SCK  As per labeling on NodeMCU

 int servoPin=D8;
 Servo Servo1;
 
const byte n_rows = 4;
const byte n_cols = 4;
 String otp="";
 int otp_len=0;
char keys[n_rows][n_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
 byte colPins[n_rows] = {D3, D9,D0,9};
byte rowPins[n_cols] = {D7, D6, D5, D4};
 
Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols); 
 
void setup(){

 Servo1.attach(servoPin); 
  Serial.begin(115200);
  Serial.println("Initializing OLED Display");
  display.init();
          
 
  display.flipScreenVertically();
 // display.setFont(ArialMT_Plain_10);
 //// display.drawString(0, 26,"hello");
      //  display.display();
        
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
      delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}
 
void loop(){
  char myKey = myKeypad.getKey();
  Servo1.write(0);
  if (myKey != NULL){
   /* if(myKey=='#')
    {
      otp[otp_len]=' ';
      Serial.println(otp);
    }
    else{*/
    Serial.print("Key pressed: ");
    Serial.println(myKey);
    otp=otp+myKey;
    otp_len++;
    display.drawString(0, 26, otp);
  display.display();
    delay(1000);
    if(otp_len==4) {
      delay(2000);
      // get value 
      Serial.print("number: ");
      Serial.println(Firebase.getInt("number"));
      int num=Firebase.getInt("number");
      String val=String(num);
      if(val==otp){
        Servo1.write(0);
        delay(1000);
        Servo1.write(180);
        Serial.println("opened");
          display.drawString(0, 26,"opened");
        display.display();
        delay(5000);
        Servo1.write(0);
        display.clear();
        Serial.println("closed");
          display.drawString(0, 26,"closed");
        display.display();
        delay(5000);
      otp_len=0;
      display.clear();
      otp="";
        
      }
      else{
        display.clear();
        display.drawString(0, 26,"wrong otp::");
        display.display();
        delay(2000);
      otp_len=0;
      display.clear();
      otp="";
      display.drawString(0, 26,"enter otp:");
    //display.update();
  // write the buffer to the display
  display.display();
      }
    }
        
      
    else{
     // clear the display
  display.clear();
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, otp);
    //display.update();
  // write the buffer to the display
  display.display();
  delay(1000);
    }
  }
    else if(otp_len==0){
      display.drawString(0, 26, "enter otp : ");
    //display.update();
  // write the buffer to the display
  display.display();
    }

  //drawFontFaceDemo();
  //delay(1000);
}

 
void drawFontFaceDemo() {
  // clear the display
  display.clear();
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 26, "enter otp :");
  // write the buffer to the display
  display.display();
}
