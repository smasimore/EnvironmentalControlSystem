/********** Switch.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/28/2017
 Last Updated Date: 10/29/2017
 Description: API for initializing and handling switches. 3 switches, one 
	changes edit mode, one increments the value being edited, and the last
	decrements the value being edited.
 Hardware: 
    TM4C123G LaunchPad 
    3 Switches
 Hardware Configuration:
    See diagram.
 */

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "Switch.h"
#include "ECSMain.h"

volatile static unsigned long Last;

/**************Switch_Init***************
 Description: Initializes PB0-2 to be used as switches.
 Inputs: none
 Outputs: none
*/
void Switch_Init(){
  // Init PB0-2.
  SYSCTL_RCGCGPIO_R |= 0x02;//port B
  while((SYSCTL_PRGPIO_R&0x2) == 0){};
  GPIO_PORTB_DIR_R  &= ~0x7; // PB0-2 is out 
  GPIO_PORTB_AFSEL_R &= ~0x7; // PB0-2 disable other function
  GPIO_PORTB_DEN_R |= 0x7; // PB0-2 is enabled as digital i/o
  GPIO_PORTB_PCTL_R &= ~0x0FFF; // PB0-2 GPIO
  GPIO_PORTB_AMSEL_R = 0; // PB0-2 not analog
  GPIO_PORTB_IS_R &= ~0x7; // PB0-2 edge-sensitive
  GPIO_PORTB_IBE_R |= 0x7; // PB0-2 both edges
  GPIOPortBArm();
  
  // Init Timer0 for debounding.
  SYSCTL_RCGCTIMER_R |= 0x01;   // Activate TIMER0
  while((SYSCTL_RCGCTIMER_R & 0x00000001) == 0){};
  Last = GPIO_PORTB_DATA_R; // Initial switch state
 }

 /**************GPIOPortBArm***************
 Description: Arms Port B interrupt.
 Inputs: none
 Outputs: none
*/
static void GPIOPortBArm(){
  GPIO_PORTB_ICR_R = 0x7; // Clear flags
  GPIO_PORTB_IM_R |= 0x7; // Arm interrupt on PB4-0
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)|0x000000A000; // Priority 5
  NVIC_EN0_R |= 1<<0x1; // Enable interrupt 4 in NVIC  
}

 /**************Timer0Arm***************
 Description: Arms Timer0 interrupt to debounce switches. Interrupts after 
    10ms.
 Inputs: none
 Outputs: none
*/
static void Timer0Arm(){
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x0000002;    // 3) this is yelling at me so make it periodic
  TIMER0_TAILR_R = 160000;      // 4) 10ms reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R |= 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R |= 0x00000001;    // 10) enable TIMER0A
}

 /**************GPIOPortB_Handler***************
 Description: Interrupts on rising or falling edge of Port B. Disarms Port B
    interrupt and enables Timer0 interrupt.
 Inputs: none
 Outputs: none
*/
void GPIOPortB_Handler(){
  GPIO_PORTB_ICR_R = 0x1F; // Acknowledge.
  GPIO_PORTB_IM_R &= ~0x1F; // Disarm interrupt on PB4-0 
  if (Last == 0) {    // 0 means it was previously released, so this is a press
    ButtonPressed();  // execute user task
  }
  Timer0Arm(); // Start timer, once interrupts will re-arm Port B.
}

 /**************Timer0A_Handler***************
 Description: Disables Timer0 interrupt and re-arms Port B interrupt.
 Inputs: none
 Outputs: none
*/
void Timer0A_Handler(){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// aAknowledge TIMER2A timeout
  TIMER0_IMR_R = 0x00000000;    // Disarm timeout interrupt
  Last = GPIO_PORTB_DATA_R; // Switch state
  GPIOPortBArm(); // Re-arm interrupt on PB0-2.
}

 /**************ButtonPressed***************
 Description: Determines which button was pressed and updates the relevant 
	ECSMain global variables.
 Inputs: none
 Outputs: none
*/
void ButtonPressed(){
  uint32_t data = GPIO_PORTB_DATA_R;
  switch (data){
    // PB0 pressed, change edit mode
    case 0x01:
      ECSMain_EditMode++;
      ECSMain_EditMode = (int)ECSMain_EditMode < 3 
        ? ECSMain_EditMode 
        : EM_NONE; 
      break;
    default:
      break;

    // PB1 pressed, decrement limit
    case 0x02:
      if (ECSMain_EditMode == EM_SOFT_LIMIT) {
        ECSMain_SoftLimit = ECSMain_SoftLimit == 0 
          ? 0
          : ECSMain_SoftLimit - 1;
      } else if (ECSMain_EditMode == EM_HARD_LIMIT) {
        ECSMain_HardLimit = ECSMain_HardLimit == ECSMain_SoftLimit
          ? ECSMain_SoftLimit
          : ECSMain_HardLimit - 1;
      }
      break;

    // PB2 pressed, increment limit
    case 0x04:
      if (ECSMain_EditMode == EM_SOFT_LIMIT) {
        ECSMain_SoftLimit = ECSMain_SoftLimit == ECSMain_HardLimit 
          ? ECSMain_HardLimit
          : ECSMain_SoftLimit + 1;
      } else if (ECSMain_EditMode == EM_HARD_LIMIT) {
        ECSMain_HardLimit = ECSMain_HardLimit == 999
          ? 999
          : ECSMain_HardLimit + 1;
      }
      break;
  } 
}
