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
    Backlight (pin 10) connected to +3.3 V
    MISO (pin 9) unconnected 
    SCK (pin 8) connected to PA2 (SSI0Clk)
    MOSI (pin 7) connected to PA5 (SSI0Tx)
    TFT_CS (pin 6) connected to PA3 (SSI0Fss)
    CARD_CS (pin 5) unconnected
    Data/Command (pin 4) connected to PA6 (GPIO)
    RESET (pin 3) connected to PA7 (GPIO)
    VCC (pin 2) connected to +3.3 V
    Gnd (pin 1) connected to ground
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
