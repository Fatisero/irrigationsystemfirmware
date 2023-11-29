
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

#define MAX_WATER_LEVEL_CM     (22.8)
#define MIN_WATER_LEVEL_CM     (0.0)
#define MAX_WATER_IN_LETTERS   (4.0)

class reservoir {
	public : 
	
	reservoir(int tri , int echo) ; 
	
	void ActivateAndReadUltrasonicSensor() ;
	
	float getWaterLevel() ;
	
	float getWaterInLetter() ;
	
	private :
		long duration ; //us 
		float distance ; //cm 
		float Waterlevel ;//in %
    float WaterInLetter ;
		
		int trigPin ;
		int echoPin ;
 
} ;

