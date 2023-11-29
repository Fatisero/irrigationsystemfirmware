#include "reservoir.h"
#include "SoilMoistureSensor.h"
#include "DHT.h"

#include <WiFi.h>
#include <HTTPClient.h>
//192.168.137.1  //ipv4   ,32 bits ,  2^(32)  ,0000000000000000000000000 
String HOST_NAME   = "http://192.168.137.1:8000"; // CHANGE IT
String PATH_NAME   = "/app";      // CHANGE IT

#define DHTPIN 4  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

const char* ssid = "DESKTOP-QOPKUBM";
const char* password = "*2774D3x";

int numberOfResults = 3; // Number of results to be read
int fieldNumber = 1; // Field number which will be read out

const int trigPin = 5;
const int echoPin = 18;

const int adcPin = 36 ; // gpio2

reservoir   res(trigPin ,echoPin ) ;
SoilMoistureSensor SoilSensor(adcPin) ;

float WaterinLiter  ;
int soilMoistureADC  ;  //declar

DHT dht(DHTPIN, DHTTYPE);// Initialize DHT sensor.

void setup() {
  
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  dht.begin();

    while(!Serial){delay(100);}

    // We start by connecting to a WiFi network
  
    Serial.println();
    Serial.println("******************************************************");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
}


void loop() {
  
  res.ActivateAndReadUltrasonicSensor() ;

  Serial.print("WaterLevel ");
  Serial.println(res.getWaterLevel()) ;
  WaterinLiter = res.getWaterInLetter();
  Serial.print("Water in Letter ");
  Serial.println(res.getWaterInLetter()) ;//*
 
 int soilmoisture = SoilSensor.getValueSoilMoisture() ;
  printf("pr Soil Moisture ADC %d \r\n" , soilmoisture) ;

// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

 
  String AllValues = "{ \"id\" :"+String( ESP.getEfuseMac()) + " , \"WatterinLiter\":" + String(WaterinLiter ) +", \"SoilMoisture\":" +
               String(soilmoisture)+" ,"+ "\"Humidity\":" + String(h)+ "," + "\"Temperature\":" + String(t)+"}";
  
  HTTPClient http;

  http.begin(HOST_NAME + PATH_NAME);
  http.addHeader("Content-Type", "application/json");

  //int httpCode = http.POST(queryString);
  int httpCode = http.POST(AllValues);
    
      // httpCode will be negative on error
  if (httpCode > 0) {
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();

 // char ssid[23];
 // snprintf(ssid, 23, "MCUDEVICE-%llX", ESP.getEfuseMac());
  //D4739F29E748
  //CCDA9F29E748  esp32  board 1
 // printf ("esp id : %s\r\n" , ssid) ;
  delay(3000);
}







