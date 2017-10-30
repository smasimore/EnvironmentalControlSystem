#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "LED.h"

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

void LED_GreenOn() {
  // Yellow and red off
  GPIO_PORTF_DATA_R &= ~0x18;
  // Green on
  GPIO_PORTF_DATA_R |= 0x2;

}
  
void LED_YellowOn() {
  // Green and red off
  GPIO_PORTF_DATA_R &= ~0x12;
  // Yellow on
  GPIO_PORTF_DATA_R |= 0x8;

}
  
void LED_RedOn() {
  // Green and yellow off
  GPIO_PORTF_DATA_R &= ~0x8;
  // Red on
  GPIO_PORTF_DATA_R |= 0x10;
}

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

static void testWait() {
  for(int j = 0; j < 8000000; j++) {};
}
