#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__ 

// Error codes for the soil moisture sensor
#define ERROR_SOIL_SENSOR_IN_AIR                   (-100) // Error: Soil moisture sensor is in the air
#define ERROR_SOIL_SENSOR_SHORT_CIRCUITED          (-101) // Error: Short circuit in the soil moisture sensor
#define ERROR_SOIL_SENSOR_IN_WATER                 (-102) // Error: Soil moisture sensor is in the water

// Error codes for the ultrasonic sensor
#define ERROR_ULTRASONIC_SENSOR_PASSED_MAX_DETECTION (-200) // Error: Ultrasonic sensor passed maximum detection distance


#endif /*__ERROR_CODES_H__*/
