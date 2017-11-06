/********** LED.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/05/2017
 Description: API for initializing and controling the green, yellow, and red
	LEDs.
 Hardware:
    TM4C123G LaunchPad  
		Green, Yellow, and Red LEDs
 Hardware Configuration: 
   See schematic.
 */

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "LED.h"

/**************LED_Init***************
 Description: Inits PF1 (green), PF3 (yellow), and PF4 (red).
 Inputs: none
 Outputs: none
*/
void LED_Init() {
  SYSCTL_RCGCGPIO_R |= 0x20; // Init clock for Port F
  while ((SYSCTL_RCGCGPIO_R&0x20) == 0) {}; // Wait for clock to init 
  GPIO_PORTF_DIR_R |= 0x1A; // PF1,3,4 outputs
  GPIO_PORTF_DEN_R |= 0x1A; // PF1,3,4 enable digital input
  GPIO_PORTF_AFSEL_R &= ~0x1A; // PF1,3,4 not alternative
  GPIO_PORTF_AMSEL_R &= ~0x1A;    // PF1,3,4 no analog
  GPIO_PORTF_PCTL_R &= ~0xFF0F0;   // PF1,3,4 gpio
  GPIO_PORTF_DATA_R = ~0x1A; // PF1,3,4 off
}

/**************LED_GreenOn***************
 Description: Turns green on and yellow and red off.
 Inputs: none
 Outputs: none
*/
void LED_GreenOn() {
  // Yellow and red off
  GPIO_PORTF_DATA_R &= ~0x18;
  // Green on
  GPIO_PORTF_DATA_R |= 0x2;

}

/**************LED_YellowOn***************
 Description: Turns yellow on and green and red off.
 Inputs: none
 Outputs: none
*/
void LED_YellowOn() {
  // Green and red off
  GPIO_PORTF_DATA_R &= ~0x12;
  // Yellow on
  GPIO_PORTF_DATA_R |= 0x8;
}
  
/**************LED_RedOn***************
 Description: Turns red on and green and yellow off.
 Inputs: none
 Outputs: none
*/
void LED_RedOn() {
  // Green and yellow off
  GPIO_PORTF_DATA_R &= ~0xA;
  // Red on
  GPIO_PORTF_DATA_R |= 0x10;
}

/**************LED_Test***************
 Description: Test function that cycles through green, yellow, and red LEDs.
 Inputs: none
 Outputs: none
*/
void LED_Test() {
  LED_Init();
  
  while (1) {
    LED_GreenOn();
    testWait();
    LED_YellowOn();
    testWait();
    LED_RedOn();
    testWait();
  }
}

/**************testWait***************
 Description: Helper function to wait between test cases.
 Inputs: none
 Outputs: none
*/
static void testWait() {
  for(int j = 0; j < 8000000; j++) {};
}
