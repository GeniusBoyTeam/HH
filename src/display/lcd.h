#if !defined(HH_LCD)
#define HH_LCD

#include <Arduino.h>
#include <Adafruit_ILI9341.h>

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

struct refreshVal
{
    double x;
    double y;
    double z;
    double a;
    double f;
    int alarm;
    int memoryMount;
};

struct stringProperties
{

};

static refreshVal currentValues;
static refreshVal lastValues;
static Adafruit_ILI9341 *display = NULL;
void displaySetup(void);
Adafruit_ILI9341 *initDisplay(char rotate);

#endif // HH_LCD