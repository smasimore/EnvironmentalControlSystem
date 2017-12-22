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
#include "ST7735.h"
#include "LCD.h"
#include "ECSMain.h"

void drawSoftLimitSection(int16_t color, int softLimit);
void drawHardLimitSection(int16_t color, int hardLimit);
void drawCO2LevelSection(int co2Val);
void drawProducingO2Section(void);

// PRIVATE VARS

const int16_t DefaultColor = ST7735_CYAN;
const int16_t EditingLimitsColor = ST7735_MAGENTA;


/**************LCD_Init***************
 Description: Inits ST7735.
 Inputs: none
 Outputs: none
*/
void LCD_Init() {
  ST7735_InitR(INITR_REDTAB);
}

/**************LCD_Update***************
 Description: Updates LCD based on ECSMain's global vars.
 Inputs:
  softLimit: int cached soft limit for system
  hardLimit: int cached hard limit for system
  co2Val: int cached CO2 value
 Outputs: none
*/
void LCD_Update(int softLimit, int hardLimit, int co2Val) {
  drawSoftLimitSection(
    ECSMain_EditMode == EM_SOFT_LIMIT ? EditingLimitsColor : DefaultColor,
    softLimit
  );
  drawHardLimitSection(
    ECSMain_EditMode == EM_HARD_LIMIT ? EditingLimitsColor : DefaultColor,
    hardLimit
  );
  ST7735_DrawString(1, 6, (uint8_t *)"___________________", DefaultColor);
  
  drawCO2LevelSection(co2Val);
  drawProducingO2Section();
}

/**************drawSoftLimitSection***************
 Description: Draws soft limit label and value on LCD.
  color: Color to draw soft limit. Changes based on edit mode.
  softLimit: int soft limit value.
 Outputs: none
*/
void drawSoftLimitSection(int16_t color, int softLimit) {
  ST7735_DrawString(2, 2, (uint8_t *)"Soft Limit:", color);
  
  uint8_t softLimitArr[6];
  softLimitArr[5] = '\0';
  softLimitArr[4] = '%';
  softLimitArr[3] = softLimit % 10 + '0';
  softLimit /= 10;
  softLimitArr[2] = '.';
  softLimitArr[1] = softLimit % 10 + '0';
  softLimit /= 10;  
  softLimitArr[0] = (softLimit % 10) == 0 ? ' ' : (softLimit % 10)+ '0';
  
  ST7735_DrawString(14, 2, softLimitArr, color);
}

/**************drawHardLimitSection***************
 Description: Draws hard limit label and value on LCD.
  color: Color to draw hard limit. Changes based on edit mode.
  hardLimit: int hard limit value.
 Outputs: none
*/
void drawHardLimitSection(int16_t color, int hardLimit) {
  ST7735_DrawString(2, 3, (uint8_t *)"Hard Limit:", color);
  
  uint8_t hardLimitArr[6];
  hardLimitArr[5] = '\0';
  hardLimitArr[4] = '%';
  hardLimitArr[3] = hardLimit % 10 + '0';
  hardLimit /= 10;
  hardLimitArr[2] = '.';
  hardLimitArr[1] = hardLimit % 10 + '0';
  hardLimit /= 10;  
  hardLimitArr[0] = (hardLimit % 10) == 0 ? ' ' : (hardLimit % 10)+ '0';
  
  ST7735_DrawString(14, 3, hardLimitArr, color);
}

/**************drawCO2LevelSection***************
 Description: Draws CO2 level with one digit to left or decimal and two to the right.
 Inputs:
  co2Val: Current CO2 value to draw.
 Outputs: none
*/
void drawCO2LevelSection(int co2Val) {
  int16_t color;
  if (ECSMain_State->led == LED_GREEN) {
    color = ST7735_GREEN;
  } else if (ECSMain_State->led == LED_YELLOW) {
    color = ST7735_YELLOW;
  } else {
    color = ST7735_RED;
  }
  
  ST7735_DrawString(6, 8, (uint8_t *)"CO2 Level", color);
  
  char thirdDigit = co2Val % 10 + '0';
  co2Val /= 10;
  char secondDigit = co2Val % 10 + '0';
  co2Val /= 10;
  char firstDigit = (co2Val % 10) == 0 ? ' ' : co2Val % 10 + '0';
  
  ST7735_DrawChar(10, 100, firstDigit, color, ST7735_BLACK, 3);
  ST7735_DrawChar(30, 100, secondDigit, color, ST7735_BLACK, 3);
  ST7735_DrawChar(50, 100, '.', color, ST7735_BLACK, 3);
  ST7735_DrawChar(70, 100, thirdDigit, color, ST7735_BLACK, 3);
  ST7735_DrawChar(90, 100, '%', color, ST7735_BLACK, 3);
}

/**************drawProducingO2Section***************
 Description: If electrolysis on, print message.
 Inputs:
 Outputs: none
*/
void drawProducingO2Section() {
  if (ECSMain_State->electrolysis == ELEC_ON) {
    ST7735_DrawString(2, 4, (uint8_t *)"Producing O2...", DefaultColor);
  } else {
    ST7735_DrawString(2, 4, (uint8_t *)"               ", DefaultColor);
  }
}

// TEST FUNCTION

/**************LCD_Test***************
 Description: Initializes LCD and updates LCD for testing purposes.
 Inputs:
 Outputs: none
*/
void LCD_Test() {
  LCD_Init();
  LCD_Update(ECSMain_SoftLimit, ECSMain_HardLimit, ECSMain_CO2Val);
}
