/********** LED.h ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/14/2017
 Description: API for initializing and controling the green, yellow, and red
  LEDs.
 Hardware:
    TM4C123G LaunchPad 
    Green, Yellow, and Red LEDs
 Hardware Configuration: 
   See schematic.
 */

// PUBLIC FUNCTIONS

/**************LED_Init***************
 Description: Inits PF1 (green), PF3 (yellow), and PF4 (red).
 Inputs: none
 Outputs: none
*/
void LED_Init(void);

/**************LED_GreenOn***************
 Description: Turns green on and yellow and red off.
 Inputs: none
 Outputs: none
*/
void LED_GreenOn(void);

/**************LED_YellowOn***************
 Description: Turns yellow on and green and red off.
 Inputs: none
 Outputs: none
*/
void LED_YellowOn(void);

/**************LED_RedOn***************
 Description: Turns red on and green and yellow off.
 Inputs: none
 Outputs: none
*/
void LED_RedOn(void);

// TEST FUNCTIONS

/**************LED_Test***************
 Description: Test function that cycles through green, yellow, and red LEDs.
 Inputs: none
 Outputs: none
*/
void LED_Test(void);

static void testWait(void);
