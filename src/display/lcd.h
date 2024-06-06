#if !defined(HH_LCD)
#define HH_LCD

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Fonts/FreeSerifBold24pt7b.h>
#include <Fonts/FreeSerifBold18pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <iostream>
#include <list>
#include "main.h"

#define LCD_MOSI 39
#define LCD_MISO 40
#define LCD_CS 38
#define LCD_DC 37
#define LCD_CLK 36
#define LCD_RST 35
#define LCD_BACKLIGHT 41

enum EChar
{
    X = 0,
    Y = 1,
    Z = 2,
    A = 3,
    F = 4
};

enum EState
{
    Idle = 0,
    Run,
    Jog,
    Alarm,
    Door,
    Homing,
    Hold,
    Check,
    Cycle,
    Sleep,
    Startup // extra state for refresh state on startup
};

struct refreshVal
{
    std::string x;
    std::string y;
    std::string z;
    std::string a;
    std::string f;
    EState state;
    int memoryMount;
};

struct stringProperties
{
};

static Adafruit_ILI9341 *display = NULL;
void displaySetup(void);
void drawMainTheme(void);
Adafruit_ILI9341 *initDisplay(char rotate);

#endif // HH_LCD