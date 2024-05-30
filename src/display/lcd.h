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

static Adafruit_ILI9341 *display = NULL;
void displaySetup(void);
Adafruit_ILI9341 *initDisplay(char rotate);

#endif // HH_LCD