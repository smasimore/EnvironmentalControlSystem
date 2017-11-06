/********** Lab9PrepMain.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 11/06/2017
 Last Updated Date: 11/06/2017
 Description: Main controller for Lab9. Initializes modules, collects ADC
	data, and prints to UART. There are 3 ways to collect data using the ADC:
		1) Periodic interrupt using SysTick or timer, ISR triggers the ADC sample 
			 and waits for it to complete (when channel in ADC0_RIS_R == 1). ISR 
			 stores result.
		2) Hardware timer triggers and begins ADC sample. ADC is armed to trigger 
			 an interrupt when it completes. ISR stores result.
		3) In main process trigger ADC sample and wait for it to complete (when 
			 channel in ADC0_RIS_R == 1).
	 We will be using #2.
 Hardware: 
   TM4C
 */

#include <stdint.h>
#include <assert.h>
#include "Lab9PrepMain.h"
#include "PLL.h"
#include "LED.h"
#include "Switch.h"
#include "LCD.h"
#include "ADC.h"
#include "FIFO.h"
#include "UART.h"

#define NUM_SAMPLES 100

txDataType Samples[NUM_SAMPLES];
            
int main() {
  PLL_Init(Bus80MHz);
	TxFifo_Init();
	UART_Init();
	
	DisableInterrupts();
	ADC_Init();
	EnableInterrupts();
	
	collectDataFromFIFO();
	printToUART();
  
  return 0;
}

/**************collectDataFromFIFO***************
 Description: Waits until data in FIFO then puts in array. Returns when array
		is full.
 Inputs: none
 Outputs: none
*/
static void collectDataFromFIFO() {
	int count = 0;
	txDataType * data;
  while (count < NUM_SAMPLES) {
		if (TxFifo_Size() > 0) {
			TxFifo_Get(data);
			Samples[count] = *data;
			count++;
		}
  }
}

/**************printToUART***************
 Description: Prints array to UART.
 Inputs: none
 Outputs: none
*/
static void printToUART() {
	for (int i = 0; i < NUM_SAMPLES; i++) {
		UART_OutString("    ");
		UART_OutUDec((int)Samples[i]);
	}
}
