#include "lcd.h"

Adafruit_ILI9341 *initDisplay(char rotate)
{
    SPIClass *vspi = NULL;
    pinMode(LCD_BACKLIGHT, OUTPUT);
    digitalWrite(LCD_BACKLIGHT, HIGH);
    vspi = new SPIClass(FSPI);
    vspi->begin(LCD_CLK, -1, LCD_MOSI, -1); // SCLK, MISO, MOSI, SS
    Adafruit_ILI9341 *a = new Adafruit_ILI9341(vspi, LCD_DC, LCD_CS, LCD_RST);
    a->begin();
    a->fillScreen(ILI9341_RED);
    a->setRotation(rotate);
    display = a;
    return a;
}

void overWritePosition(EChar character ,double value)
{
    
}

void refresh(void)
{
    if (currentValues.x != lastValues.x)
    {
        lastValues.x = currentValues.x;
        overWritePosition(X,currentValues.x);
    }
    
}

void displayTask(void *p)
{
    initDisplay(2);
    while (true)
    {
        refresh();
        vTaskDelay(1);
    }
}


void displaySetup(void)
{
    xTaskCreate(displayTask, "displayTask", 10 * 1024, NULL, 1, NULL);
}