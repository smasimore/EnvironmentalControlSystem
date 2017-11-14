/********** CO2Sensor.h ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/05/2017
 Description: API for initializing and sampling CO2 sensor. Uses PE5, ADC0, and 
	Timer1A. Modified from ValvanoWare.
 Hardware: 
		TM4C123G LaunchPad 
		MQ-135 CO2 Sensor	
 Hardware Configuration: 
   See schematic.
 */

// PUBLIC FUNCTIONS

/**************CO2Sensor_Init***************
 Description: Inits PE5, ADC0, and Timer1A.
 Inputs: none
 Outputs: none
*/
void CO2Sensor_Init(void);

/**************CO2Sensor_Init***************
 Description: Converts ADC value to % CO2.
 Inputs: none
 Output: Int between 0 and 999 representing 0.00% to 9.99% CO2 in the air.
*/
int CO2Sensor_ADCToPercCO2(int adcVal);

// PRIVATE FUNCTIONS

void initPortAndADC(void);
void initTimer1A(void);
int getADCVal(void);

// TEST FUNCTIONS

/**************CO2Sensor_Test***************
 Description: Gets ADC value, waits, and repeats.
	global variable.
 Inputs: none
 Outputs: none
*/
void CO2Sensor_Test(void);

static void testWait(void);
