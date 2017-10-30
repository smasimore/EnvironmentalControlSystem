
// PUBLIC FUNCTIONS

void LCD_Init(void);
void LCD_Update(void);

// PRIVATE FUNCTIONS

void drawSoftLimitSection(int16_t color, int softLimit);
void drawHardLimitSection(int16_t color, int hardLimit);
void drawCO2LevelSection(int co2Val, int softLimit, int hardLimit);
void drawProducingO2Section(void);

// TEST FUNCTIONS

void LCD_Test(void);
