
#include "Arduino.h"

#define ADC_VAL_SENSOR_IN_OPEN_AIR_MIN      120
#define ADC_VAL_SENSOR_IN_OPEN_AIR_MAX      200
#define ADC_VAL_SENSOR_SHORT_CIRCUIT        3000
#define ADC_VAL_SENSOR_IN_SOIL_MAX          2900
#define ADC_VAL_SENSOR_IN_SOIL_MIN          1350

class SoilMoistureSensor {
	
  public :
	//constructor 
	SoilMoistureSensor (int adcPin); 
	
	
	//methods
	int getValueSoilMoisture();
	
	private :
	//propriet~s
	
	int SensorADCPin ;

} ;