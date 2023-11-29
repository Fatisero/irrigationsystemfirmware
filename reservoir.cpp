#include "arduino.h"
#include "reservoir.h"
#include "ErrorCodes.h"

reservoir::reservoir(int tri , int echo)
{
	 pinMode(tri, OUTPUT); // Sets the trigPin as an Output
   pinMode(echo, INPUT); // Sets the echoPin as an Input
   this->trigPin = tri ;
	 this->echoPin = echo ;
}

void reservoir::ActivateAndReadUltrasonicSensor() 
{
  // Clears the trigPin
  digitalWrite(this->trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(this->trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->trigPin, LOW);

  this->duration = pulseIn(this->echoPin, HIGH);
  this->distance = this->duration * SOUND_SPEED/2;
}

float reservoir::getWaterLevel() 
{
  this->Waterlevel = 100 - ((this->distance - MIN_WATER_LEVEL_CM) / (MAX_WATER_LEVEL_CM - MIN_WATER_LEVEL_CM)) * 100 ;
  return this->Waterlevel ;
}

float reservoir::getWaterInLetter() 
{
  this->WaterInLetter = (this->Waterlevel  / 100) * MAX_WATER_IN_LETTERS ;
  return this->WaterInLetter ;
}

