
// PUBLIC FUNCTIONS
void CO2Sensor_Init(void);
int CO2Sensor_ADCToPercCO2(int adcVal);

// PRIVATE FUNCTIONS

void initPE5(void);
void initADC0(void);
void initTimer1A(void);
int getADCVal(void);

// TEST FUNCTIONS

void CO2Sensor_Test(void);
static void testWait(void);
