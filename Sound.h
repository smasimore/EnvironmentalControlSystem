/********** Sound.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 10/29/2017
 Description: API initializing sound port and playing sound. Uses PB0 for
		output to speaker, SysTick timer determine when to output to PB0, and
		Timer2 to cycle through an array of notes to play. These notes determine
		the frequency SysTick timer sends output to PB0.
 Hardware: 
    TM4C123G LaunchPad 
    PN2222
    1N914
    Speaker
 Hardware Configuration:
		See diagram.
 */
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#define E1   (1896<<5)   // 1318.5 Hz

// PUBLIC FUNCTIONS

/**************Sound_Init***************
 Description: Initializes sound module, including PE5 for output to speaker,
		SysTick for sending output to PE5 at specified frequencies, and Timer2 
		for cycling through notes to play (defines frequency SysTick interrupts at).
 Inputs: none
 Outputs: none
*/
void Sound_Init(void);

/**************Sound_On***************
 Description: Starts sound from beginning by enabling SysTick and Timer2.
 Inputs: none
 Outputs: none
*/
void Sound_On(void);

/**************Sound_Off***************
 Description: Stops sound.
 Inputs: none
 Outputs: none
*/
void Sound_Off(void);

// PRIVATE FUNCTIONS

void SysTick_Handler(void);
void Timer2_Handler(void);
void playSound(void);
void soundOut(uint32_t data);

// TEST FUNCTIONS
void Sound_Test(void);
static void testWait(void);;
