
#include "Arduino.h"
// Define ADC threshold values for different conditions
#define ADC_VAL_SENSOR_IN_OPEN_AIR_MIN      120
#define ADC_VAL_SENSOR_IN_OPEN_AIR_MAX      200
#define ADC_VAL_SENSOR_SHORT_CIRCUIT        3000
#define ADC_VAL_SENSOR_IN_SOIL_MAX          2900
#define ADC_VAL_SENSOR_IN_SOIL_MIN          1350

class SoilMoistureSensor {
	
  public :
    // Constructor: Initializes the SoilMoistureSensor object with the specified ADC pin.
	SoilMoistureSensor (int adcPin); 
	
    // Method to get the soil moisture value
	int getValueSoilMoisture();
	
	private :

    // Property to store the ADC pin connected to the sensor
	int SensorADCPin ;

} ;

