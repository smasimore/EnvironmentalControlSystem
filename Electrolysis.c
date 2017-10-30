#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "Electrolysis.h"

// PUBLIC FUNCTIONS

void Electrolysis_Init() {
	SYSCTL_RCGCGPIO_R |= 0x20; // Init clock for Port F
	while ((SYSCTL_RCGCGPIO_R&0x20) == 0) {}; // Wait for clock to init
	
	GPIO_PORTF_DIR_R |= 0x4; // PF1 output
	GPIO_PORTF_DEN_R |= 0x4; // PF1 enable digital input
	GPIO_PORTF_AFSEL_R &= ~0x4; // PF1 not alternative
	GPIO_PORTF_AMSEL_R &= ~0x4;  	// PF1 no analog
	GPIO_PORTF_PCTL_R &= ~0x4;   // PF1 gpio
	GPIO_PORTF_DATA_R &= ~0x4; // PF1 off	
}

void Electrolysis_On() {
	GPIO_PORTF_DATA_R |= 0x4;
}

void Electrolysis_Off() {
	GPIO_PORTF_DATA_R &= ~0x4;
}

// TEST FUNCTIONS

void Electrolysis_Test() {
	Electrolysis_Init();
	
	while (1) {
		Electrolysis_On();
		testWait();
		
		Electrolysis_Off();
		testWait();
	}
}

static void testWait() {
	for(int j = 0; j < 80000000; j++) {};
}
