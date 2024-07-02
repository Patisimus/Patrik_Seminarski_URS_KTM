

#include "stm32f4xx_hal.h"


#define DEVICE_ADDR (0x27 << 1) // Adresa uređaja

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETDDRAMADDR 0x80


#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON 0x04
#define LCD_CURSOROFF 0x00
#define LCD_BLINKOFF 0x00


#define LCD_DISPLAYMOVE 0x08
#define LCD_MOVELEFT 0x00


#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x8DOTS 0x00
#define LCD_1LINE 0x00


#define LCD_BACKLIGHT 0x08


#define ENABLE 0x04


#define RS 0x01

void LCDIspis_Init(uint8_t rows);
void LCDIspis_Clear();
void LCDIspis_Home();
void LCDIspis_Display();
void LCDIspis_ScrollDisplayLeft();
void LCDIspis_SetCursor(uint8_t col, uint8_t row);
void LCDIspis_PrintStr(const char c[]);

