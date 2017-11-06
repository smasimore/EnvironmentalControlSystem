/********** ECSMain.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/05/2017
 Description: Main controller for ECS. Initializes modules and transitions
	system between states. Test mode managed through TestMode variable to
	test each module independently.
 Hardware: 
   See schematic.
 Hardware Configuration: 
   See schematic.
 */

#include <stdint.h>
#include <assert.h>
#include "ECSMain.h"
#include "PLL.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "Electrolysis.h"
#include "CO2Sensor.h"
#include "LCD.h"

// TEST MODE

// TM_OFF: None, regular run mode
// TM_STATES: Cycles system through states
// TM_LEDS: Calls LED_Test, cycles through LED's.
// TM_SOUND: Calls Sound_Test, cyles through sound on/off.
// TM_ELEC: Calls Electrolysis_Test, cycles through elec on/off.
// TM_CO2SENSOR: Calls CO2Sensor_Test, samples ADC once.
// TM_LCD: Calls LCD_Test, inits and updates LCD once.
enum TestMode {TM_OFF, TM_STATES, TM_LEDS, TM_SOUND, 
               TM_ELEC, TM_CO2SENSOR, TM_LCD} TestMode = TM_OFF;

// PUBLIC GLOBAL VARS
            
const ECSState_t * ECSMain_State;                
int ECSMain_CO2ADCVal; // 0 - 4095
int ECSMain_SoftLimit; // 0 - 1000, resolution of .1
int ECSMain_HardLimit; // 0 - 1000, resolution of .1
EditMode_t ECSMain_EditMode = EM_NONE;
int ECSMain_CO2Val; // 0 - 1000, resolution of .1
int ECSMain_PrevCO2ADCVal;
int ECSMain_PrevCO2Val;
int ECSMain_PrevSoftLimit;
int ECSMain_PrevHardLimit;
int ECSMain_PrevEditMode;

// PRIVATE GLOBAL VARS

// Represents CO2 level < soft limit state.
const ECSState_t ECSStateNoLimitPassed = {
  LED_GREEN, 
  SOUND_OFF, 
  ELEC_OFF,
};

// Represents CO2 level >= soft limit && CO2 level < hard limit state.
const ECSState_t ECSStateSoftLimitPassed = {
  LED_YELLOW, 
  SOUND_OFF, 
  ELEC_ON,
};

// Represents CO2 level >= hard limit state.
const ECSState_t ECSStateHardLimitPassed = {
  LED_RED, 
  SOUND_ON, 
  ELEC_ON,
};

const ECSState_t * PrevECSState;

int main() {
  PLL_Init(Bus80MHz);
  
	// Handle test modes.
  if (TestMode != TM_OFF) {
    mainTest();
    return 0;
  }
  
	// Initialize system.
  initECSState();
  initECSModules();
	updateECSState(ECSMain_SoftLimit, ECSMain_HardLimit);
	updateECSSystem(ECSMain_SoftLimit, ECSMain_HardLimit, ECSMain_CO2Val);
  
  while (1) {
		updateECS();
  }
  
  return 0;
}

// PRIVATE FUNCTIONS

/**************initECSState***************
 Description: Set initial state for system. Set previous state vars to initial
  state.
 Inputs: none
 Outputs: none
*/
void initECSState() {
  ECSMain_State = &ECSStateNoLimitPassed;
  ECSMain_SoftLimit = 20;
  ECSMain_HardLimit = 50;
  ECSMain_EditMode = EM_NONE;
  ECSMain_CO2ADCVal = 0;
  ECSMain_CO2Val = 0;
  
  // Set prev variables to track state changes.
  updatePrevValues(ECSMain_CO2ADCVal);
}

/**************initECSModules***************
 Description: Initialize each module.
 Inputs: none
 Outputs: none
*/
void initECSModules() {
  DisableInterrupts();
  LED_Init();
  Switch_Init();
  Sound_Init();
  Electrolysis_Init();
  CO2Sensor_Init();
  LCD_Init();
  EnableInterrupts();
}

/**************updateECS***************
 Description: Update ECS based on changes to global vars.
 Inputs: none
 Outputs: none
*/
void updateECS() {
	// Cache current values to avoid critical sections.
	int currCO2ADCVal = ECSMain_CO2ADCVal;
  int softLimit = ECSMain_SoftLimit;
  int hardLimit = ECSMain_HardLimit;
  int co2Val = ECSMain_CO2Val;
	
	// If limits or CO2 level has changed, update ECS state.
	if (softLimit != ECSMain_PrevSoftLimit ||
			hardLimit != ECSMain_PrevHardLimit ||
			currCO2ADCVal != ECSMain_PrevCO2ADCVal) {
			ECSMain_CO2Val = CO2Sensor_ADCToPercCO2(currCO2ADCVal);
		updateECSState(softLimit, hardLimit);
	}
	
	// If state has changed, update system. If not, still need to update LCD.
	if (ECSMain_State != PrevECSState) {
		updateECSSystem(softLimit, hardLimit, co2Val);
	} else {
		LCD_Update(softLimit, hardLimit, co2Val);
	}
		
	updatePrevValues(currCO2ADCVal);
}

/**************updatePrevValues***************
 Description: Set previous state values to current state. Need to pass in CO2
	ADC value since using global var will result in a critical section.
 Inputs: 
	currCO2ADCVal: int containing cached CO2 ADC value
 Outputs: none 
*/
void updatePrevValues(int currCO2ADCVal) {
  PrevECSState = ECSMain_State;
  ECSMain_PrevCO2ADCVal = currCO2ADCVal;
  ECSMain_PrevCO2Val = ECSMain_CO2Val;
  ECSMain_PrevSoftLimit = ECSMain_SoftLimit;
  ECSMain_PrevHardLimit = ECSMain_HardLimit;
  ECSMain_PrevEditMode = ECSMain_EditMode;
}

/**************updateECSState***************
 Description: Based on current CO2 level and soft/hard limits, update the ECS
  state.
 Inputs: 
	softLimit: int cached soft limit for system
	hardLimit: int cached hard limit for system
 Outputs: none
*/
void updateECSState(int softLimit, int hardLimit) {
  if (ECSMain_CO2Val < ECSMain_SoftLimit) {
    ECSMain_State = &ECSStateNoLimitPassed;
  } else if (ECSMain_CO2Val >= ECSMain_SoftLimit && 
             ECSMain_CO2Val < ECSMain_HardLimit) {
    ECSMain_State = &ECSStateSoftLimitPassed;
  } else {
    ECSMain_State = &ECSStateHardLimitPassed;
  }
}

/**************updateECSSystem***************
 Description: Update electrolysis, LED, sound, and LCD.
 Inputs:
	softLimit: int cached soft limit for system
	hardLimit: int cached hard limit for system
	co2Val: int cached CO2 value
 Outputs: none
*/
void updateECSSystem(int softLimit, int hardLimit, int co2Val) {
  updateElectrolysis();
  updateLED();
  updateSound();
  LCD_Update(softLimit, hardLimit, co2Val);
}  

/**************updateElectrolysis***************
 Description: Based on system state, updates electrolysis state.
 Inputs: none
 Outputs: none
*/
void updateElectrolysis() {
  if (ECSMain_State->electrolysis == ELEC_ON) {
    Electrolysis_On();
  } else {
    Electrolysis_Off();
  }
}

/**************updateLED***************
 Description: Based on system state, updates LED state.
 Inputs: none
 Outputs: none
*/
void updateLED() {
  // Disable interrupts since Sound.c uses same Port F to avoid a critical
  // section.
  DisableInterrupts();
  switch (ECSMain_State->led) {
    case LED_GREEN:
      LED_GreenOn();
      break;
    case LED_YELLOW:
      LED_YellowOn();
      break;
    case LED_RED:
      LED_RedOn();
      break;
  }
  EnableInterrupts();
}

/**************updateSound***************
 Description: Based on system state, updates sound state.
 Inputs: none
 Outputs: none
*/
void updateSound() {
  if (ECSMain_State->sound == SOUND_ON) {
    Sound_On();
  } else {
    Sound_Off();
  }
}

// TEST VARS AND FUNCTIONS

/**************mainTest***************
 Description: Based on test mode, either execute global testStates or runs
	module-specifid tests.
 Inputs: none
 Outputs: none
*/
void mainTest() {
  initECSState();
  switch (TestMode) {
    case TM_STATES:
      testStates();
      break;
    case TM_LEDS:
      LED_Test();
      break;
    case TM_SOUND:
      Sound_Test();
      break;
    case TM_ELEC:
      Electrolysis_Test();
      break;
    case TM_CO2SENSOR:
      CO2Sensor_Test();
      break;
    case TM_LCD:
      LCD_Test();
      break;
    default:
      break;
  }
}

const int testStatesCasesSize = 9;
const TestVars_t testStatesCases[9] = {
  {20, 50, EM_NONE, 10, &ECSStateNoLimitPassed},
  {20, 50, EM_SOFT_LIMIT, 10, &ECSStateNoLimitPassed},
  {20, 50, EM_HARD_LIMIT, 10, &ECSStateNoLimitPassed},
  {20, 50, EM_NONE, 30, &ECSStateSoftLimitPassed},
  {20, 50, EM_SOFT_LIMIT, 30, &ECSStateSoftLimitPassed},
  {20, 50, EM_HARD_LIMIT, 30, &ECSStateSoftLimitPassed},
  {20, 50, EM_NONE, 60, &ECSStateHardLimitPassed},
  {20, 50, EM_SOFT_LIMIT, 60, &ECSStateHardLimitPassed},
  {20, 50, EM_HARD_LIMIT, 60, &ECSStateHardLimitPassed},
};

/**************testStates***************
 Description: Goes through test scenarios defined in testStatesCases and
	verifies state transitions are correct. Additionally this test is an
	integration test with all modules initialized and running for each state.
 Inputs: none
 Outputs: none
*/
void testStates(){
  initECSState();
  initECSModules();
  
  while (1) {
    for (int i = 0; i < testStatesCasesSize; i++) {
      TestVars_t testVars = testStatesCases[i];
      testHelperOverrideVars(testVars);
      updateECSState(ECSMain_SoftLimit, ECSMain_HardLimit);
      updateECSSystem(ECSMain_SoftLimit, ECSMain_HardLimit, ECSMain_CO2Val);
      assert(testVars.expectedState == ECSMain_State);  
      testWait();
    }
  }
}

/**************testHelperOverrideVars***************
 Description: Overrides global vars with test vars.
 Inputs: none
 Outputs: none
*/
void testHelperOverrideVars(TestVars_t testVars) {
  ECSMain_SoftLimit = testVars.softLimit;
  ECSMain_HardLimit = testVars.hardLimit;
  ECSMain_EditMode = testVars.editMode;
  ECSMain_CO2Val = testVars.co2Val;
}

/**************testWait***************
 Description: Helper function to wait between test cases.
 Inputs: none
 Outputs: none
*/
static void testWait() {
  for(int j = 0; j < 8000000; j++) {};
}
