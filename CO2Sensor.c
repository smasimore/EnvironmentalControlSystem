// Modified from Valvano code

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "CO2Sensor.h"
#include "ECSMain.h"

#define ADC_AVGING 6

// init port, adc, timer to sample
void CO2Sensor_Init() {
	initPE5();
	initADC0();
	initTimer1A();
}

// 0 - 1000
int CO2Sensor_ADCToPercCO2(int adcVal) {
	// TODO: Calibrate
	return 0;
}

// PRIVATE FUNCTIONS

void initPE5() {
  SYSCTL_RCGCGPIO_R |= 0x10; // Activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x10) != 0x10){};
  GPIO_PORTE_DIR_R &= ~0x20; // Make PE5 input
  GPIO_PORTE_AFSEL_R |= 0x20; // Enable alternate function on PE5
  GPIO_PORTE_DEN_R &= ~0x20; // Disable digital I/O on PE5
  GPIO_PORTE_AMSEL_R |= 0x20; // Enable analog functionality on PE5
}

void initADC0() {
  SYSCTL_RCGCADC_R |= 0x0001; // Activate ADC0 
  ADC0_PC_R &= ~0xF; // Clear max sample rate field
  ADC0_PC_R |= 0x1; // Configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123; // Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008; // Disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;  // Seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F; // clear SS3 field
  ADC0_SSMUX3_R += 8; // Set channel
  ADC0_SSCTL3_R = 0x0006; // No TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008; // Disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008; // Enable sample sequencer 3
	ADC0_SAC_R = ADC_AVGING;
}

void initTimer1A() {
  SYSCTL_RCGCTIMER_R |= 0x02; // Activate TIMER1 (bit 1)
	while((SYSCTL_RCGCTIMER_R & 0x00000002) == 0){}; // Wait until Timer1 is clocked
  TIMER1_CTL_R = 0; // Disable TIMER1A during setup
  TIMER1_CFG_R = 0; // Configure for 32-bit mode
  TIMER1_TAMR_R = 0x2; // Configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = 4000000-1; // Reload value (50ms)
  TIMER1_TAPR_R = 0; // Bus clock resolution
  TIMER1_ICR_R = 0x1; // Clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x1; // Arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00006000; // Priority 3
  NVIC_EN0_R |= 1<<21; // Enable IRQ 21 in NVIC
	TIMER1_CTL_R = 0x00000001;
}

void Timer1A_Handler() {
	TIMER1_ICR_R = TIMER_ICR_TATOCINT; // Acknowledge
	ECSMain_CO2ADCVal = getADCVal();
}

int getADCVal() {
	int adcVal;
	ADC0_PSSI_R = 0x0008; // Initiate SS3
  while((ADC0_RIS_R&0x08)==0){}; // Wait for conversion done
  adcVal = ADC0_SSFIFO3_R&0xFFF; // Read result
  ADC0_ISC_R = 0x0008; // Acknowledge completion

	return adcVal;
}

// TEST FUNCTIONS

void CO2Sensor_Test() {
	initPE5();
	initADC0();
	
	int testADCVal;
	while (1) {
		testADCVal = getADCVal();
		testWait();
	}
}

static void testWait() {
	for(int j = 0; j < 80000000; j++) {};
}
