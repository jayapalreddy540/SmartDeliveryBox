#include <Servo.h>
#include <ESP8266WiFi.h>
#include <Keypad.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <FirebaseArduino.h>  // version ArduinoJson 5.9.0
// Set these to run example.
#define FIREBASE_HOST "smartdeliverybox-5dc37.firebaseio.com"

#define FIREBASE_AUTH "feKZ2pK7m1fUyE2rQBwa9TWVZoXNPdOLVXVcxFMM"


#define WIFI_SSID "jai"  
#define WIFI_PASSWORD "qwertyuiop"
#define PATH "orders/uid"

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D1, D2);  //D2=SDK  D1=SCK  As per labeling on NodeMCU

 int servoPin=D8;
 Servo Servo1;

 void printScreen(String m);
 
const byte n_rows = 4;
const byte n_cols = 4;
 String otp="";
 int otp_len=0;
 char  a[10];
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
   Servo1.write(0); 
   Serial.begin(115200);
   Serial.println("Initializing OLED Display");
   display.init();
        display.flipScreenVertically();
         display.setTextAlignment(TEXT_ALIGN_LEFT);
         display.setFont(ArialMT_Plain_24);
        printScreen("smart delivery box");
        delay(2000);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.print("connecting");
   while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
       printScreen("connecting....");
      delay(500);
    }
    printScreen("connected");
    Serial.println();
    Serial.print("connected to ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop()
{
  while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
       printScreen("Reconnecting....");
      delay(500);
    }
  char myKey = myKeypad.getKey();
  Servo1.write(0);
  if (myKey != NULL)
  {
    Serial.print("Key pressed: ");
    Serial.println(myKey);
    otp=otp+myKey;
    otp_len++;
    printScreen(otp);
    delay(1000);
    if(otp_len==4)
    {
      delay(2000);
      // get value 
      Serial.print("uid : ");
      Serial.print(Firebase.getString("orders/uid"));
      String uid=Firebase.getString("orders/uid");
      Serial.print("number: ");
      Serial.print(Firebase.getInt("users/"+uid+"/otp"));
      int num=Firebase.getInt("users/"+uid+"/otp");
      String val=String(num);
      if(val==otp)
      {
        Servo1.write(0);
        delay(1000);
        Servo1.write(180);
        Serial.println("opened");
          printScreen("opened");
        delay(5000);
        Servo1.write(0);
        Serial.println("closed");
          printScreen("closed");
        delay(5000);
        otp_len=0;
        otp="";
       }
      else
      {
        printScreen("wrong otp!!");
        delay(2000);
        otp_len=0;
        otp="";
        printScreen("Enter otp");
      }
    }
        
      
    else
    {
     printScreen(otp);
     delay(1000);
    }
  }
    else if(otp_len==0)
    {
      printScreen("enter otp =");
    }

}

void printScreen(String msg)
{
display.clear();
display.drawString(0,26,msg);
display.display();
}
