/********** Switch.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 09/16/2017
 Last Updated Date: 09/27/2017
 Description: API for initializing and handling interrupt for 5 switches used
    by alarm clock. Handler checks which switch was hit and modifies relevant
    global vars created in ClockMain.c. Handles debouncing by starting Timer0B
    on switch edge interrupt and recording the state of the switch after a 
    certain time period. Modified from original by Jonathan Valvano.
 Hardware: 
    TM4C123G LaunchPad 
    Sitronix ST7735R 18-bit color 1.8" TFT LCD
    5 Switches
    PN2222
    1N914
    Speaker
 Hardware Configuration:
    See diagram.
 */
#include <stdint.h>

// PUBLIC FUNCTIONS

/**************Switch_Init***************
 Description: Initializes PB0-2 to be used as switches.
 Inputs: none
 Outputs: none
*/
void Switch_Init(void);

// PRIVATE FUNCTIONS

static void GPIOPortBArm(void);
static void Timer0Arm(void);
void GPIOPortB_Handler(void);
void Timer0A_Handler(void);
void ButtonPressed(void);
