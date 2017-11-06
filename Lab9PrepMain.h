/********** Lab9PrepMain.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 11/06/2017
 Last Updated Date: 11/06/2017
 Description: Main controller for Lab9. Initializes modules, collects ADC
		data, and prints to UART.
 Hardware: 
   TM4C
 */

// PRIVATE FUNCTIONS

void DisableInterrupts(void);
void EnableInterrupts(void);

static void collectDataFromFIFO(void);
static void printToUART(void);
