#include <stdint.h>
#include "ST7735.h"
#include "LCD.h"
#include "ECSMain.h"

const int16_t DefaultColor = ST7735_CYAN;
const int16_t EditingLimitsColor = ST7735_MAGENTA;


void LCD_Init() {
	ST7735_InitR(INITR_REDTAB);
	// draw all sections
}


/**
	Soft Limit: XX.X% (purple when editing)
	Hard Limit: YY.Y% (purple when editing)

	CURRENT CO2 LEVEL: ZZ.Z% (green, yellow, or red)

	Producing O2...

**/

void LCD_Update() {
	// Create a copy of global vars to prevent critical section.
	EditMode_t editMode = ECSMain_EditMode;
	int softLimit = ECSMain_SoftLimit;
	int hardLimit = ECSMain_HardLimit;
	int co2Val = ECSMain_CO2Val;

	drawSoftLimitSection(
		editMode == EM_SOFT_LIMIT ? EditingLimitsColor : DefaultColor,
		softLimit
	);
	drawHardLimitSection(
		editMode == EM_HARD_LIMIT ? EditingLimitsColor : DefaultColor,
		hardLimit
	);
	ST7735_DrawString(1, 6, (uint8_t *)"___________________", DefaultColor);
	
	drawCO2LevelSection(co2Val, softLimit, hardLimit);
	drawProducingO2Section();
}

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


void drawCO2LevelSection(int co2Val, int softLimit, int hardLimit) {
	int16_t color;
	if (co2Val < softLimit) {
		color = ST7735_GREEN;
	} else if (co2Val >= softLimit && co2Val < hardLimit) {
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

void drawProducingO2Section() {
	if (ECSMain_State->electrolysis == ELEC_ON) {
		ST7735_DrawString(2, 4, (uint8_t *)"Producing O2...", DefaultColor);
	}
}

// TEST FUNCTION

void LCD_Test() {
	LCD_Init();
	LCD_Update();
}
