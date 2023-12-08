

#include "reservoir.h"
#include "SoilMoistureSensor.h"
#include "DHT.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MQTTPubSubClient.h>

#define DEBUG       0


#define DHTPIN 4  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
#define BUTTON  0

const char* ssid = "DESKTOP-QOPKUBM";
const char* pass = "*2774D3x";


//192.168.137.1  //ipv4   ,32 bits ,  2^(32)  ,0000000000000000000000000 
String HOST_NAME   = "http://192.168.137.1:8000"; // CHANGE IT
String PATH_NAME   = "/app";      // CHANGE IT

WiFiClient client;
MQTTPubSubClient mqtt;


int numberOfResults = 3; // Number of results to be read
int fieldNumber = 1; // Field number which zzzwill be read out

uint32_t chipID;

const int pumpePin = 13;

const int trigPin = 5;
const int echoPin = 18;

const int adcPin = 36 ; // gpio2

reservoir   res(trigPin ,echoPin ) ;
SoilMoistureSensor SoilSensor(adcPin) ;

float WaterinLiter  ;
float WaterLevel ;
int soilMoistureADC  ;  //declar

DHT dht(DHTPIN, DHTTYPE);// Initialize DHT sensor.

String deviceId ;

StaticJsonDocument<128> doc;

uint32_t timer = 0 ;

void callback_g(const char* topic, const char* payload, const size_t len)
{
  Serial.println(len) ;
}

void connect() {
connect_to_wifi:
    Serial.print("connecting to wifi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected!");

connect_to_host:
    Serial.print("connecting to host...");
    client.stop();
    while (!client.connect("192.168.1.17", 1883)) {
        Serial.print(".");
        delay(1000);
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi disconnected");
            goto connect_to_wifi;
        }
    }
    Serial.println(" connected!");

    Serial.print("connecting to mqtt broker...");
    mqtt.disconnect();
    while (!mqtt.connect("arduino", "public", "public")) {
        Serial.print(".");
        delay(1000);
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi disconnected");
            goto connect_to_wifi;
        }
        if (client.connected() != 1) {
            Serial.println("WiFiClient disconnected");
            goto connect_to_host;
        }
    }
    Serial.println(" connected!");
}

void setup() {
  
  //timer = millis() ;
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  pinMode( BUTTON , INPUT_PULLUP ) ;
  pinMode(pumpePin, OUTPUT);
  dht.begin();
    
    while(!Serial){delay(100);}

    // We start by connecting to a WiFi network
  #ifdef DEBUG  
    Serial.println();
    Serial.println("******************************************************");
    Serial.print("Connecting to ");
    Serial.println(ssid);
 #endif /*DEBUG*/

  chipID = ESP.getEfuseMac() ;

  deviceId = String(chipID) ;
    
  WiFi.begin(ssid, pass);

   // initialize mqtt client
    mqtt.begin(client);

    // subscribe callback which is called when every packet has come
   /* mqtt.subscribe([](const String& topic, const String& payload, const size_t size) {
        Serial.println("mqtt received: " + topic + " - " + payload);
    });*/
    mqtt.subscribe(callback_g) ;

    // subscribe topic and callback which is called when /hello has come
   /* mqtt.subscribe("/irrigationsystem/devices/"+deviceId +"commands" , [](const String& payload, const size_t size) {
       // Serial.print("/hello ");
        Serial.println(payload);
    });*/

    connect();
    //timer = millis() ;

    Serial.print("Subscribed on topic :") ;
    Serial.println("/irrigationsystem/devices/"+deviceId +"commands") ;
}
 


void loop() {


if(millis() - timer > 15000)
{
  if( digitalRead(BUTTON) == 0)
  {  
    #ifdef DEBUG 
     Serial.println("btn clicked") ;
     #endif /*DEBUG*/
     digitalWrite(pumpePin, HIGH);
  } else {
    // Knopf wurde losgelassen
     #ifdef DEBUG
    Serial.println("btn not clicked");
     #endif /*DEBUG*/
    digitalWrite(pumpePin, LOW); 
  }
  
  res.ActivateAndReadUltrasonicSensor() ;
 
  WaterLevel = res.getWaterLevel() ;
  WaterinLiter = res.getWaterInLetter() ;
  int soilmoisture = SoilSensor.getValueSoilMoisture() ;
  //printf("pr Soil Moisture ADC %d \r\n" , soilmoisture) ;

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    #ifdef DEBUG
    Serial.println(F("Failed to read from DHT sensor!"));
    #endif /*DEBUG*/
    //return ;
  }


  //construire le message json
  doc["id"] = String(chipID);
  doc["WatterinLiter"] = WaterinLiter;
  doc["WatterLevel"] = WaterLevel;
  doc["SoilMoisture"] = soilmoisture;
  doc["Humidity"] = h;
  doc["Temperature"] = t;

  String output;

  serializeJson(doc, output);


  String PublishTopic = "/irrigationsystem/devices/"+deviceId +"/data" ;
  Serial.println("Publish Topic :" + PublishTopic);
  mqtt.publish(PublishTopic , output) ;

  
    if (!mqtt.isConnected()) {
        connect();
    }

#ifdef DEBUG
  Serial.println("Output :" + output) ;
#endif /*DEBUG*/
  timer = millis() ;
}
 mqtt.update();  // should be called

}
