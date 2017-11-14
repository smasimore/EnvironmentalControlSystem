/********** Sound.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/14/2017
 Description: API initializing sound port and playing sound. Uses PF0 for
    output to speaker, SysTick timer determine when to output to PF0, and
    Timer2 to cycle through an array of notes to play. These notes determine
    the frequency SysTick timer sends output to PF0. Only plays 1 note for 
    alarm.
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
#include "Sound.h"
  
static uint32_t Note = 0;
static uint32_t Count = 0;
static uint32_t SpeakerData = 1;
static const uint32_t Notes[2]={E1,0}; // Only play one note for alarm.

/**************Sound_Init***************
 Description: Initializes sound module, including PF0 for output to speaker,
    SysTick for sending output to PF0 at specified frequencies, and Timer2 
    for cycling through notes to play (defines frequency SysTick interrupts at).
 Inputs: none
 Outputs: none
*/
void Sound_Init(){
  SYSCTL_RCGCGPIO_R |= 0x20; // Init clock for Port F
  while ((SYSCTL_RCGCGPIO_R&0x20) == 0) {}; // Wait for clock to init
    
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  
  GPIO_PORTF_DIR_R |= 0x1; // PF0 output
  GPIO_PORTF_DEN_R |= 0x1; // PF0 enable digital input
  GPIO_PORTF_AFSEL_R &= ~0x1; // PF0 not alternative
  GPIO_PORTF_AMSEL_R &= ~0x1;    // PF0 no analog
  GPIO_PORTF_PCTL_R &= ~0xF;   // PF0 gpio
  GPIO_PORTF_DATA_R &= ~0x1; // PF0 off
  
  // Set up systick timer for sending output to PF0 at frequency that will be 
  // defined by cycling through Notes with Timer2. Set at priority 2 so
  // the sound isnt jittery because of other interrupts.
  NVIC_ST_CTRL_R = 0; // Disable SysTick during setup
  NVIC_ST_RELOAD_R = 79999; // Set initial interrupt period (changes later)
  NVIC_ST_CURRENT_R = 0; // Any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x4000000; // Set priority 2
  
  // Init Timer2 for cycling through Notes at pre-determined rate.
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2 (bit 2)
  while((SYSCTL_RCGCTIMER_R & 0x00000004) == 0){}; // Wait until Timer2 is clocked
  TIMER2_CTL_R = 0x00000000;    // 1) disable TIMER2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = 4000000-1;    // 4) reload value(at 12.5 ns this is 50ms which is about the right speed 
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear TIMER2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
  // Interrupts enabled in the main program after all devices initialized
  // vector number 37, interrupt number 21
  NVIC_EN0_R |= 1<<23;           // 9) enable IRQ 23 in NVIC
}

/**************Sound_On***************
 Description: Starts sound from beginning by enabling SysTick and Timer2.
 Inputs: none
 Outputs: none
*/
void Sound_On(void) {
  NVIC_ST_CTRL_R = 7; 
  TIMER2_CTL_R = 0x00000001;
}

/**************Sound_Off***************
 Description: Stops sound.
 Inputs: none
 Outputs: none
*/
void Sound_Off(void) {
  NVIC_ST_CTRL_R = 0;
  TIMER2_CTL_R = 0;
}

// HANDLERS

/**************SysTick_Handler***************
 Description: Play a note when SysTick interrupt occurs. No need to acknowledge
    b/c SysTick automatically acknowledges.
 Inputs: none
 Outputs: none
*/
void SysTick_Handler(void){
  playSound();
}

/**************Timer2A_Handler***************
 Description: Set Note as next note in Notes. This is then used to define
    the frequency that SysTick interrupts and plays sound.
 Inputs: none
 Outputs: none
*/
void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT; // Acknowledge TIMER2A timeout
  Note = Notes[Count]; // Update note
  Count++; // Update count
  
  // Number of notes.
  if (Count == 2) {
    Count = 0;
  }
}

/**************playSound***************
 Description: If period is not 0, alternates PF0 output. Otherwise, PF0 output
    set to 0.
 Inputs: 
    period: Reload value for SysTick (determined by Note/Timer2). 
 Outputs: none
*/
void playSound(){
  if (Note != 0){
    NVIC_ST_RELOAD_R = Note;
    soundOut(SpeakerData);
    SpeakerData = SpeakerData ? 0 : 1;
  } else {
    soundOut(0);
  }
}

/**************soundOut***************
 Description: Based on data input, sets PF0 to 1 or 0.
 Inputs: 
    data: 1 or 0. If 1, will set PF0 high. If 0, sets low.
 Outputs: none
*/
void soundOut(uint32_t data){
  if (data) {
    GPIO_PORTF_DATA_R |= 0x1;
  } else {
    GPIO_PORTF_DATA_R &= ~0x1;
  }
}

// TEST FUNCTIONS

/**************Sound_Test***************
 Description: Test function that cycles through sound on and off.
 Inputs: none
 Outputs: none
*/
void Sound_Test() {
   Sound_Init();
    
  while (1) {
    Sound_On();
    testWait();
    
    Sound_Off();
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
