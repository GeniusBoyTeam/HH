#include "lcd.h"

refreshVal currentValues;
refreshVal lastValues;
char *enumStates[10] = {"Idle", "Run", "Jog", "Alarm", "Door", "Homing", "Hold", "Check", "Cycle", "Sleep"};

Adafruit_ILI9341 *
initDisplay(char rotate)
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

    display->setTextColor(ILI9341_BLACK);
    display->setTextSize(1);
    display->setFont(&FreeSerifBold24pt7b);
    display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 0), componentHeght, componentHeght, roundRects, ILI9341_YELLOW);
    display->setCursor(12, convertLocation(start, gap, componentHeght, 0));
    display->print("X");
    display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 1), componentHeght, componentHeght, roundRects, ILI9341_YELLOW);
    display->setCursor(12, convertLocation(start, gap, componentHeght, 1));
    display->print("Y");
    display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 2), componentHeght, componentHeght, roundRects, ILI9341_YELLOW);
    display->setCursor(12, convertLocation(start, gap, componentHeght, 2));
    display->print("Z");
    display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 3), componentHeght, componentHeght, roundRects, ILI9341_YELLOW);
    display->setCursor(12, convertLocation(start, gap, componentHeght, 3));
    display->print("A");
    lastValues.x = "disconnect";
    lastValues.y = "disconnect";
    lastValues.z = "disconnect";
    lastValues.a = "disconnect";

    display->fillRoundRect(240, 125, 75, 60, 5, ILI9341_GREENYELLOW);
    // feedRate

    display->fillRoundRect(240, 195, 75, 30, 5, ILI9341_WHITE);
    lastValues.state = Startup;
    // state

    display->drawLine(230, 0, 230, 240, ILI9341_DARKGREY);
}
int startX = 60;
void refreshXPos(void)
{
    log_v("Refresh XPos");
    int gap = 12;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->fillRoundRect(startX, convertLocation(15, gap, componentHeght, 0), 160, 32, 5, ILI9341_BLUE);
    display->setCursor(startX, convertLocation(start - 4, gap, componentHeght, 0));
    display->setTextColor(ILI9341_WHITE);
    display->print(currentValues.x.c_str());
}

void refreshYPos(void)
{
    log_v("Refresh YPos");
    int gap = 12;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->fillRoundRect(startX, convertLocation(15, gap, componentHeght, 1), 160, 32, 5, ILI9341_BLUE);
    display->setCursor(startX, convertLocation(start - 4, gap, componentHeght, 1));
    display->setTextColor(ILI9341_WHITE);
    display->print(currentValues.y.c_str());
}

void refreshZPos(void)
{
    log_v("Refresh ZPos");
    int gap = 12;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->fillRoundRect(startX, convertLocation(15, gap, componentHeght, 2), 160, 32, 5, ILI9341_BLUE);
    display->setCursor(startX, convertLocation(start - 4, gap, componentHeght, 2));
    display->setTextColor(ILI9341_WHITE);
    display->print(currentValues.z.c_str());
}

void refreshAPos(void)
{
    log_v("Refresh APos");
    int gap = 12;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->fillRoundRect(startX, convertLocation(15, gap, componentHeght, 3), 160, 32, 5, ILI9341_BLUE);
    display->setCursor(startX, convertLocation(start - 4, gap, componentHeght, 3));
    display->setTextColor(ILI9341_WHITE);
    display->print(currentValues.a.c_str());
}

void refreshState(void)
{
    if (currentValues.state != lastValues.state)
    {
        log_v("Refresh State: %i",currentValues.state);
        display->fillRoundRect(240, 195, 75, 30, 5, ILI9341_WHITE);
        display->setFont(&FreeSerifBold9pt7b);
        display->setCursor(250, 215);
        display->setTextSize(1);
        display->setTextColor(ILI9341_BLACK);
        display->print(enumStates[currentValues.state]);
    }
}

void refresh(void)
{
    log_v("Refresh Display");
    int x = currentValues.x.compare(lastValues.x);
    int y = currentValues.y.compare(lastValues.y);
    int z = currentValues.z.compare(lastValues.z);
    int a = currentValues.a.compare(lastValues.a);
    if (x != 0)
    {
        refreshXPos();
    }
    if (y != 0)
    {
        refreshYPos();
    }
    if (z != 0)
    {
        refreshZPos();
    }
    if (a != 0)
    {
        refreshAPos();
    }
    refreshState();
}

void displayTask(void *p)
{
    initDisplay(3);
    drawMainTheme();
    while (true)
    {
        refresh();
        vTaskDelay(100);
    }
}

void displaySetup(void)
{
    xTaskCreate(displayTask, "displayTask", 10 * 1024, NULL, 500, NULL);
}