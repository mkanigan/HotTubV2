//#include <ArduinoOTA.h>
//
// //nodeMCU v1.0 (black) with Arduino IDE
////stream temperature data DS18B20 with 1wire on ESP8266 ESP12-E (nodeMCU v1.0)
////shin-ajaran.blogspot.com
////nodemcu pinout https://github.com/esp8266/Arduino/issues/584
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//Test
//Def
#define myPeriodic 15 //in sec | Thingspeak pub is 15sec
int Sensor= 0;
int Led1 = 16;   // D0
int Led2 = 5;   // D2
int Beeper = 4;     // D3
float prevTemp = 0;
//

///////////////////////////////////////////////////////
//// Hot Tub V2.0
//const char* server = "api.thingspeak.com";
//String apiKey ="FVJJPKSIIUHF3FCC";
//const char* MY_SSID = "AEJF382DA"; 
//const char* MY_PWD = "JFJEJLWF##R%%$@@#DSA23";
///////////////////////////////////////////////////////



///////////////////////////////////////////////////////
// Hot Tub V1.0
const char* server = "api.thingspeak.com";
String apiKey ="6DYNUKB0NWSG6U63";
const char* MY_SSID = "AEJF382DA"; 
const char* MY_PWD = "JFJEJLWF##R%%$@@#DSA23";
///////////////////////////////////////////////////////

int sent = 0;
int val;
int tempPin = 0;

//static const uint8_t LED_BUILTIN = 16;
//static const uint8_t BUILTIN_LED = 16;
//
//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10 = 1;

void setup() {
  Serial.begin(115200);
  connectWifi();
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Beeper, OUTPUT);
}

void loop() {

int rawvoltage= analogRead(Sensor);
// float millivolts= (rawvoltage/1024.0) * 5000; // = (rawvoltage*3300/1024)*1.5
float millivolts= (rawvoltage*3300/1024)*1.5;
float kelvin= (millivolts/10) + 10; //(millivolts/100); ***I think**** was /10
//Serial.print(kelvin);
//Serial.println(" degrees Kelvin");

//Serial.println("----------------------  ");
float celsius= kelvin - 273.15;
Serial.print(celsius);
Serial.println(" degrees Celsius");

float fahrenheit= ((celsius * 9)/5 +32);
Serial.print(fahrenheit);
Serial.println(" degrees Fahrenheit");
Serial.println("----------------------  ");
delay(1000);

// If temperature is less than 50 or equal to
if (fahrenheit <= 50 ) 
{
    Serial.println("----------------------  ");
    Serial.println ("  Temperature getting cold");
    digitalWrite(Led2, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(Led2, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
    Serial.println("----------------------  ");
}

//////////////////////////////////////////////////////////////////////////////////////////


// If temperature is greater than 50 and less than 100
else if (fahrenheit > 50 && fahrenheit < 100) 
{
    Serial.println("----------------------  ");
    Serial.println ("  Temperature getting cold");
    digitalWrite(Led1, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(Beeper, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(Led1, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(Beeper, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    Serial.println("----------------------  ");
}

//////////////////////////////////////////////////////////////////////////////////////////

// If temperature is greater than 110 then flash RED LED Only

 if (fahrenheit > 100)

{
  Serial.println (" Temperature too hot");
  digitalWrite(Led2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(Led2, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
}  
    if (fahrenheit < 100) 
  {
    Serial.println ("  Temperature getting cold");
    digitalWrite(Led1, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(Beeper, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(Led1, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(Beeper, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
  }
sendTeperatureTS(celsius);
}

void connectWifi()
{
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
//  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect


void sendTeperatureTS(float temp)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(temp);
   postStr += "\r\n\r\n";
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }//end if
   sent++;
 client.stop();
}//end send




