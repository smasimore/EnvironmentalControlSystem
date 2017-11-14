/********** Electrolysis.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/05/2017
 Description: API for initializing and controling the electrolysis reaction.
 Hardware: 
	 See schematic.
 Hardware Configuration: 
   See schematic.
 */

// PUBLIC FUNCTIONS

/**************Electrolysis_Init***************
 Description: Inits PF2.
 Inputs: none
 Outputs: none
*/
void Electrolysis_Init(void);

/**************Electrolysis_On***************
 Description: Turns on electrolysis.
 Inputs: none
 Outputs: none
*/
void Electrolysis_On(void);

/**************Electrolysis_Off***************
 Description: Turns off electrolysis.
 Inputs: none
 Outputs: none
*/
void Electrolysis_Off(void);

// TEST FUNCTIONS

/**************Electrolysis_Test***************
 Description: Initializes Electrolysis and toggles on and off until stopped.
 Inputs:
 Outputs: none
*/
void Electrolysis_Test(void);

static void testWait(void);
