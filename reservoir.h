
//define sound speed in cm/uS
#define SOUND_SPEED 0.034    // Define the speed of sound in air in centimeters per microsecond      
#define CM_TO_INCH 0.393701  // Conversion factor from centimeters to inches

// Define the maximum and minimum water levels in centimeters and the maximum water volume in liters
#define MAX_WATER_LEVEL_CM     (22.8)
#define MIN_WATER_LEVEL_CM     (0.0)
#define MAX_WATER_IN_LETTERS   (4.0)

// Define the Reservoir class
class reservoir {

	public : 
  
	// Constructor: Initializes the reservoir object with the trigPin and echoPin.
	reservoir(int tri , int echo) ; 

  // Method to activate and read the ultrasonic sensor
	void ActivateAndReadUltrasonicSensor() ;
	  // Method to get the water level in percentage
	float getWaterLevel() ;
	    // Method to get the water volume in liters
	float getWaterInLetter() ;

  private:
    long duration;   // Duration of the echo pulse in microseconds
    float distance;  // Distance measured by the ultrasonic sensor in centimeters
    float Waterlevel; // Water level in percentage
    float WaterInLetter; // Water volume in liters
		
    int trigPin;     // Pin connected to the trig pin of the ultrasonic sensor
    int echoPin;     // Pin connected to the echo pin of the ultrasonic sensor
 
} ;
