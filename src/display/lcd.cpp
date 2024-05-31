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
    a->fillScreen(ILI9341_BLACK);
    a->setRotation(rotate);
    display = a;
    return a;
}

void overWritePosition(EChar character, double value)
{
}

int16_t convertLocation(int start, int gap, int componentHeight, int number)
{
    return start + (gap * number) + (componentHeight * number);
}

void drawMainTheme(void)
{
    int gap = 12;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;

    display->setTextColor(ILI9341_WHITE);
    display->setTextSize(1);
    display->setFont(&FreeSerifBold24pt7b);
    display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 0), componentHeght, componentHeght, roundRects, ILI9341_DARKGREY);
    display->setCursor(12, convertLocation(start, gap, componentHeght, 0));
    display->print("X");
    display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 1), componentHeght, componentHeght, roundRects, ILI9341_DARKGREY);
    display->setCursor(12, convertLocation(start, gap, componentHeght, 1));
    display->print("Y");
    display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 2), componentHeght, componentHeght, roundRects, ILI9341_RED);
    display->setCursor(12, convertLocation(start, gap, componentHeght, 2));
    display->print("Z");
    display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 3), componentHeght, componentHeght, roundRects, ILI9341_GREEN);
    display->setCursor(12, convertLocation(start, gap, componentHeght, 3));
    display->print("A");

    display->drawLine(230, 0, 230, 240, ILI9341_DARKGREY);

    display->setTextColor(ILI9341_WHITE);
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->setCursor(70, convertLocation(start - 4, gap, componentHeght, 0));
    display->print("5555.5");
    display->setCursor(70, convertLocation(start - 4, gap, componentHeght, 1));
    display->print("135456.5");
    display->setCursor(70, convertLocation(start - 4, gap, componentHeght, 2));
    display->print("-156.5");
    display->setCursor(70, convertLocation(start - 4, gap, componentHeght, 3));
    display->print("0.0");
}

void refresh(void)
{
    if (currentValues.x != lastValues.x)
    {
        lastValues.x = currentValues.x;
        overWritePosition(X, currentValues.x);
    }
}

void displayTask(void *p)
{
    initDisplay(3);
    drawMainTheme();
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