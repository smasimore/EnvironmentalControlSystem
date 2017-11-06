/********** ADC.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/05/2017
 Description: API for initializing ADC.
		TM4C
 */


// PUBLIC FUNCTIONS

/**************ADC_Init***************
 Description: Inits PE5, ADC0, and Timer1A.
 Inputs: none
 Outputs: none
*/
void ADC_Init(void);

// PRIVATE FUNCTIONS

void initPortAndADC(void);
void initTimer1A(void);
int getADCVal(void);
