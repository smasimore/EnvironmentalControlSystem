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

// Test mode
// TM_OFF: None, regular run mode
// TM_STATES: Cycles system through states
enum TestMode {TM_OFF, TM_STATES, TM_LEDS, TM_SOUND, 
                TM_ELEC, TM_CO2SENSOR, TM_LCD} TestMode = TM_STATES;


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

// CO2 level < soft limit
const ECSState_t ECSStateNoLimitPassed = {
  LED_GREEN, 
  SOUND_OFF, 
  ELEC_OFF,
};

// CO2 level >= soft limit && CO2 level < hard limit
const ECSState_t ECSStateSoftLimitPassed = {
  LED_YELLOW, 
  SOUND_OFF, 
  ELEC_ON,
};

// CO2 level >= hard limit
const ECSState_t ECSStateHardLimitPassed = {
  LED_RED, 
  SOUND_ON, 
  ELEC_ON,
};

const ECSState_t * PrevECSState;

int main() {
  PLL_Init(Bus80MHz);
  
  if (TestMode != TM_OFF) {
    mainTest();
    return 0;
  }
  
  initECSState();
  initECSModules();
  updateECSState();
  updateECSSystem();
  
  while (1) {
    // If limits or CO2 level has changed, update ECS state.
    if (ECSMain_SoftLimit != ECSMain_PrevSoftLimit ||
        ECSMain_HardLimit != ECSMain_PrevHardLimit ||
        ECSMain_CO2ADCVal != ECSMain_PrevCO2ADCVal) {
        ECSMain_CO2Val = CO2Sensor_ADCToPercCO2(ECSMain_CO2ADCVal);
      updateECSState();
    }
      
    if (ECSMain_State != PrevECSState) {
      updateECSSystem();
    } else {
      // If state hasn't changed, we still need to update LCD.
      LCD_Update();
    }
      
    updatePrevValues();
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
  updatePrevValues();
}

/**************initECSModules***************
 Description: Initialize each module.
  state.
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

/**************updatePrevValues***************
 Description: Set previous state values to current state.
  state.
 Inputs: none
 Outputs: none
*/
void updatePrevValues() {
  PrevECSState = ECSMain_State;
  ECSMain_PrevCO2ADCVal = ECSMain_CO2ADCVal;
  ECSMain_PrevCO2Val = ECSMain_CO2Val;
  ECSMain_PrevSoftLimit = ECSMain_SoftLimit;
  ECSMain_PrevHardLimit = ECSMain_HardLimit;
  ECSMain_PrevEditMode = ECSMain_EditMode;
}

/**************updateECSState***************
 Description: Based on current CO2 level and soft/hard limits, update the ECS
  state.
 Inputs: none
 Outputs: none
*/
void updateECSState() {
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
 Description: Update electrolysis, LED, and sound. LCD will be
  // updated in main since LCD may have to change even if state has not (e.g.
  // edit mode changing).
 Inputs: none
 Outputs: none
*/
void updateECSSystem() {
  updateElectrolysis();
  updateLED();
  updateSound();
  LCD_Update();
}  

void updateElectrolysis() {
  if (ECSMain_State->electrolysis == ELEC_ON) {
    Electrolysis_On();
  } else {
    Electrolysis_Off();
  }
}

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

void updateSound() {
  if (ECSMain_State->sound == SOUND_ON) {
    Sound_On();
  } else {
    Sound_Off();
  }
}

// TEST VARS AND FUNCTIONS

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

// integration test, go through each state
void testStates(){
  initECSState();
  initECSModules();
  
  while (1) {
    for (int i = 0; i < testStatesCasesSize; i++) {
      TestVars_t testVars = testStatesCases[i];
      testHelperOverrideVars(testVars);
      updateECSState();
      updateECSSystem();
      assert(testVars.expectedState == ECSMain_State);  
      testWait();
    }
  }
}

void testHelperOverrideVars(TestVars_t testVars) {
  ECSMain_SoftLimit = testVars.softLimit;
  ECSMain_HardLimit = testVars.hardLimit;
  ECSMain_EditMode = testVars.editMode;
  ECSMain_CO2Val = testVars.co2Val;
}

static void testWait() {
  for(int j = 0; j < 8000000; j++) {};
}
