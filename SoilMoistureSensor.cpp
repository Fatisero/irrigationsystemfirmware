#include "arduino.h"
#include "SoilMoistureSensor.h"
#include "ErrorCodes.h"

SoilMoistureSensor::SoilMoistureSensor(int adcPin)
{
  this->SensorADCPin = adcPin ;
}


int SoilMoistureSensor::getValueSoilMoisture()
{
  int ret = 0 ;
  
	int adcVal = analogRead(this->SensorADCPin) ;
	
  return adcVal ;

	if((adcVal >= ADC_VAL_SENSOR_IN_OPEN_AIR_MIN) && (adcVal <= (ADC_VAL_SENSOR_IN_OPEN_AIR_MAX) ))
	{
    ret = ERROR_SOIL_SENSOR_IN_AIR ;
	}
	else if((adcVal >= (ADC_VAL_SENSOR_SHORT_CIRCUIT -20 )) && (adcVal <= (ADC_VAL_SENSOR_SHORT_CIRCUIT +20)) )
	{
		ret = ERROR_SOIL_SENSOR_SHORT_CIRCUITED ;
	}
	
  //remove next comment 
	/*else if( (adcVal >= ADC_VAL_SENSOR_IN_SOIL_MIN) && (adcVal <= ADC_VAL_SENSOR_IN_SOIL_MAX) )
	{
		ret = (int) map(adcVal, ADC_VAL_SENSOR_IN_SOIL_MIN, ADC_VAL_SENSOR_IN_SOIL_MAX, 0, 100) ;
	}*/

  return ret ;
	
}

