#include "lcd_biblioteka.h"
extern I2C_HandleTypeDef hi2c1;






uint8_t dpFunction;
uint8_t dpControl;
uint8_t dpMode;
uint8_t dpRows;
uint8_t dpBacklight;



void SendCommand(uint8_t);
void SendChar(uint8_t);
void Send(uint8_t, uint8_t);
void Write4Bits(uint8_t);
void ExpanderWrite(uint8_t);
void PulseEnable(uint8_t);
void DelayInit(void);
void DelayUS(uint32_t);





void LCDIspis_Init(uint8_t rows)
{
    dpRows = rows;
    dpBacklight = LCD_BACKLIGHT;
    dpFunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

    if (dpRows > 1)
    {
        dpFunction |= LCD_2LINE;
    }

    DelayInit();
    HAL_Delay(50);

    ExpanderWrite(dpBacklight);
    HAL_Delay(1000);

    Write4Bits(0x03 << 4);
    DelayUS(4500);

    Write4Bits(0x03 << 4);
    DelayUS(4500);

    Write4Bits(0x03 << 4);
    DelayUS(4500);

    Write4Bits(0x02 << 4);
    DelayUS(100);

    SendCommand(LCD_FUNCTIONSET | dpFunction);

    dpControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCDIspis_Display();
    LCDIspis_Clear();

    dpMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    SendCommand(LCD_ENTRYMODESET | dpMode);
    DelayUS(4500);

    LCDIspis_Home();
}




void LCDIspis_Clear()
{
    SendCommand(LCD_CLEARDISPLAY);
    DelayUS(2000);
}




void LCDIspis_Home()
{
    SendCommand(LCD_RETURNHOME);
    DelayUS(2000);
}




void LCDIspis_SetCursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row >= dpRows)
    {
        row = dpRows - 1;
    }
    SendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}



void LCDIspis_Display()
{
    dpControl |= LCD_DISPLAYON;
    SendCommand(LCD_DISPLAYCONTROL | dpControl);
}



void LCDIspis_ScrollDisplayLeft(void)
{
    SendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}



void LCDIspis_PrintStr(const char c[])
{
    while (*c)
    {
        SendChar(*c++);
    }
}



void SendCommand(uint8_t cmd)
{
    Send(cmd, 0);
}



void SendChar(uint8_t ch)
{
    Send(ch, RS);
}



void Send(uint8_t value, uint8_t mode)
{
    uint8_t highnib = value & 0xF0;
    uint8_t lownib = (value << 4) & 0xF0;
    Write4Bits((highnib) | mode);
    Write4Bits((lownib) | mode);
}



void Write4Bits(uint8_t value)
{
    ExpanderWrite(value);
    PulseEnable(value);
}



void ExpanderWrite(uint8_t _data)
{
    uint8_t data = _data | dpBacklight;
    HAL_I2C_Master_Transmit(&hi2c1, DEVICE_ADDR, (uint8_t*)&data, 1, 10);
}



void PulseEnable(uint8_t _data)
{
    ExpanderWrite(_data | ENABLE);
    DelayUS(20);

    ExpanderWrite(_data & ~ENABLE);
    DelayUS(20);
}



void DelayInit(void)
{
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    DWT->CYCCNT = 0;

    __ASM volatile("NOP");
    __ASM volatile("NOP");
    __ASM volatile("NOP");
}



void DelayUS(uint32_t us)
{
    uint32_t cycles = (SystemCoreClock / 1000000L) * us;
    uint32_t start = DWT->CYCCNT;
    volatile uint32_t cnt;

    do
    {
        cnt = DWT->CYCCNT - start;
    }
    while (cnt < cycles);
}
