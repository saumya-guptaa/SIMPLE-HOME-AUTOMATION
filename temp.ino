#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"


#define LED0 2
#define DHTTYPE DHtemp1   // DHT 11

// Replace with your network details
const char* ssid = "Redmi 4";
const char* password = "********";

// Web Server on port 80
ESP8266WebServer server(80); 

// DHT Sensor
const int DHTPin2 = 4;

float temp;
float pretemp = 0;

// Initialize DHT sensor.
DHT dht1(DHTPin1, DHTTYPE);

static char celsiusTemp1[7];

void CheckWiFiConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
        digitalWrite(LED0, !HIGH);
        delay(250);
        digitalWrite(LED0, !LOW);
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    }
  }

//handles server request and responses

void ontwo(){
  digitalWrite(2,LOW);
  String s = "true";
  s = "{\"fans\" : true}";
  server.send(200, "text/plain", s);
}

void offtwo(){
  digitalWrite(2,HIGH);
  String s = "false";
  s = "{\"fans\" : false}";
  server.send(200, "text/plain", s);
}


void handleRoot() {
  String s = "you did not input anything";
  server.send(200, "text/plain", s);
}

void query(){
  int stat = digitalRead(2);
  //Serial.println(stat);
  String s;
  if(stat == 0)
    s = "{\"fans\" : true,";
  else if(stat == 1)
    s = "{\"fans\" : false,";
  
  s += String("\"location_id\": \"GH1 Roomno 202b\"") + String(",");
  s += String("\"temperature\":") + String(temp) +String("}");
  server.send(200,"text/plain",s);
  
}

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(sensor1, INPUT); // declare sensor as input
  

  dht1.begin();

  // Connecting to WiFi network
  if(WiFi.status() == WL_CONNECTED)
    {
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      delay(50);
    }
  WiFi.begin("Redmi 4", "*********");

  // Printing Message For User
  Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

  CheckWiFiConnectivity();
  Serial.println("connectedr");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/fan/0",ontwo);
  server.on("/fan/1",offtwo);
  server.on("/query",query);
  server.begin();
  Serial.println("HTTP server started");
}

// runs over and over again
void loop() {
 
  // Read temperature as Celsius (the default)
  temp = dht1.readTemperature();
  
  if (!isnan(temp)) 
    pretemp = temp;
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(temp)) {
    temp = pretemp;
    Serial.println("Failed to read from DHT sensor!");
          
  }
  else{
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C ");
    

  }
  delay(1000);
  server.handleClient();
}   
