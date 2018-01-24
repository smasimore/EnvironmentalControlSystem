/********** Electrolysis.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/14/2017
 Description: API for initializing and controlling the electrolysis reaction.
 Hardware: 
   See schematic.
 Hardware Configuration: 
   See schematic.
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Electrolysis.h"

static void testWait(void);

// PUBLIC FUNCTIONS

/**************Electrolysis_Init***************
 Description: Inits PF2.
 Inputs: none
 Outputs: none
*/
void Electrolysis_Init() {
  SYSCTL_RCGCGPIO_R |= 0x20; // Init clock for Port F
  while ((SYSCTL_RCGCGPIO_R&0x20) == 0) {}; // Wait for clock to init
  
  GPIO_PORTF_DIR_R |= 0x4; // PF2 output
  GPIO_PORTF_DEN_R |= 0x4; // PF2 enable digital input
  GPIO_PORTF_AFSEL_R &= ~0x4; // PF2 not alternative
  GPIO_PORTF_AMSEL_R &= ~0x4;    // PF2 no analog
  GPIO_PORTF_PCTL_R &= ~0x4;   // PF2 gpio
  GPIO_PORTF_DATA_R &= ~0x4; // PF2 off  
}

/**************Electrolysis_On***************
 Description: Turns on electrolysis.
 Inputs: none
 Outputs: none
*/
void Electrolysis_On() {
	GPIO_PORTF_DATA_R &= ~0x4;
}

/**************Electrolysis_Off***************
 Description: Turns off electrolysis.
 Inputs: none
 Outputs: none
*/
void Electrolysis_Off() {
  GPIO_PORTF_DATA_R |= 0x4;
}

// TEST FUNCTIONS

/**************Electrolysis_Test***************
 Description: Initializes Electrolysis and toggles on and off until stopped.
 Inputs:
 Outputs: none
*/
void Electrolysis_Test() {
  Electrolysis_Init();
  
  while (1) {
    Electrolysis_On();
    testWait();
    
    Electrolysis_Off();
    testWait();
  }
}

/**************testWait***************
 Description: Helper function to wait between test cases.
 Inputs: none
 Outputs: none
*/
static void testWait() {
  for(int j = 0; j < 80000000; j++) {};
}
