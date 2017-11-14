/********** LCD.c ************** 
 Name: Sarah Masimore (sm66498) and Sam Harper (slh3927)
 Lab Number: Officially 16415, in reality MW 9-10:30am
 TA: Andrew Lynch  
 Created Date: 10/29/2017
 Last Updated Date: 11/14/2017
 Description: API for initializing and updating LCD.
 Hardware: 
   ST7735
 Hardware Configuration: 
   See schematic.
 */

#include <stdint.h>

// PUBLIC FUNCTIONS

/**************LCD_Init***************
 Description: Inits ST7735.
 Inputs: none
 Outputs: none
*/
void LCD_Init(void);

/**************LCD_Update***************
 Description: Updates LCD based on ECSMain's global vars.
 Inputs:
  softLimit: int cached soft limit for system
  hardLimit: int cached hard limit for system
  co2Val: int cached CO2 value
 Outputs: none
*/
void LCD_Update(int softLimit, int hardLimit, int co2Val);

// PRIVATE FUNCTIONS

void drawSoftLimitSection(int16_t color, int softLimit);
void drawHardLimitSection(int16_t color, int hardLimit);
void drawCO2LevelSection(int co2Val);
void drawProducingO2Section(void);

// TEST FUNCTIONS

/**************LCD_Test***************
 Description: Initializes LCD and updates LCD for testing purposes.
 Inputs:
 Outputs: none
*/
void LCD_Test(void);
