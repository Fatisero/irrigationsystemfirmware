#include "reservoir.h"
#include "SoilMoistureSensor.h"
#include "DHT.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define DEBUG 0

#define DHTPIN 4      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11
#define BUTTON 0

const char *ssid = "DESKTOP-QOPKUBM";
const char *pass = "*2774D3x";
const char *mqtt_server = "192.168.1.17";

WiFiClient espClient;
PubSubClient client(espClient);

int numberOfResults = 3; // Number of results to be read
int fieldNumber = 1;     // Field number which will be read out

uint32_t chipID;

const int pumpePin = 13;

const int trigPin = 5;
const int echoPin = 18;

const int adcPin = 36; // gpio2

reservoir res(trigPin, echoPin);
SoilMoistureSensor SoilSensor(adcPin);

float WaterinLiter;
float WaterLevel;
int soilMoistureADC; //declared

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor.

String deviceId;

StaticJsonDocument<128> doc;
StaticJsonDocument<200>  InpDoc  ;

uint32_t timer = 0;

// Function declarations
void setup_wifi();
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();
void publishMessage(const char* topic, const char* message);
void publishSensorData();
void processIncomingMessage(char* topic, char* payload);

void setup()
{
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  pinMode( BUTTON , INPUT_PULLUP ) ;
  pinMode(pumpePin, OUTPUT);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  chipID = ESP.getEfuseMac();
  deviceId = String(chipID) ;
}

void loop()
{
  /*
  if( digitalRead(BUTTON) == 0)
  {  
    #ifdef DEBUG 
     Serial.println("btn clicked") ;
     #endif
     digitalWrite(pumpePin, HIGH);
  } else {
    // Knopf wurde losgelassen
     #ifdef DEBUG
    //Serial.println("btn not clicked");
     #endif
    digitalWrite(pumpePin, LOW); 
  }
*/

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (millis() - timer > 40000)
  {
    //publishMessage("/irrigationsystem/devices/2670323528/data", "helloXWord");
    publishSensorData();
    timer = millis();
  }
}

void setup_wifi()
{
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  Serial.print("Payload: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Verarbeiten Sie die eingehende Nachricht
  processIncomingMessage(topic, (char*)payload);
}

void processIncomingMessage(char* topic, char* payload)
{
  Serial.print("Processing incoming message on topic: ");
  Serial.println(topic);

  Serial.print("Payload: ");
  Serial.println(payload);

  DeserializationError  error = deserializeJson(InpDoc, payload);
  
  if(error.code() == DeserializationError::Ok )
  {/*
    Serial.println(F("Deserialization succeeded"));
     Serial.print("pumpestatus :");
    //  Serial.println(InpDoc["pumpe"]);
    String resp = InpDoc["pumpe"] ;
     Serial.println(resp);
*/
Serial.println(F("Deserialization succeeded"));
//Serial.print(F("pumpestatus: "));
//String resp = InpDoc["pumpe"] ;
//Serial.println(resp);

   if (InpDoc["pumpe"].is<String>()) {
  String resp = InpDoc["pumpe"].as<String>();
  Serial.print(F("pumpestatus: "));
  Serial.println(resp);

  if(resp.equals(String("ON")))
  {
    Serial.println(F("starts"));
     digitalWrite(pumpePin, HIGH);
     //Serial.println(F("starts"));
  }
  else if(resp.equals(String("OFF")))
  {
    Serial.println(F("stop"));
    digitalWrite(pumpePin, LOW);
  }
  else{

  }
} else {
  Serial.println(F("Error: 'pumpe' is not a string."));
}
/*
if (InpDoc.containsKey("pumpe")) {
  // Der Schlüssel "pumpe" existiert im JSON
  // Fahren Sie mit der Deserialisierung fort

  
} else {
  Serial.println(F("Error: 'pumpe' key not found in JSON."));
}*/

  }
  else{

  }
  // Hier können Sie die Logik für die Verarbeitung der eingehenden Nachricht hinzufügen
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("esp32Client"))
    {
      Serial.println("connected");
      client.subscribe("testTopic"); // Subscribe to the desired topic
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishSensorData()
{
  res.ActivateAndReadUltrasonicSensor();

  WaterLevel = res.getWaterLevel();
  WaterinLiter = res.getWaterInLetter();
  int soilmoisture = SoilSensor.getValueSoilMoisture();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
#ifdef DEBUG
    Serial.println(F("Failed to read from DHT sensor!"));
#endif /*DEBUG*/
  }

  // Build the JSON message
  doc["id"] = String(chipID);
  doc["WatterinLiter"] = WaterinLiter;
  doc["WatterLevel"] = WaterLevel;
  doc["SoilMoisture"] = soilmoisture;
  doc["Humidity"] = h;
  doc["Temperature"] = t;

  String output;
  serializeJson(doc, output);

  String testTopic = "/irrigationsystem/devices/" + deviceId + "/data";
  Serial.println("Publish Topic: " + testTopic);
  client.publish(testTopic.c_str(), output.c_str());

  if (!client.connected())
  {
    reconnect();
  }

#ifdef DEBUG
  Serial.println("Output: " + output);
#endif /*DEBUG*/
}

void publishMessage(const char* topic, const char* message)
{
  if (client.connected())
  {
    Serial.println("Publishing message...");
    client.publish(topic, message);
    Serial.println("Message published!");
  }
  else
  {
    Serial.println("Client not connected. Unable to publish message.");
  }
}