#if !defined(HH_LCD)
#define HH_LCD

#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include <Fonts/FreeSerifBold18pt7b.h>
#include <Fonts/FreeSerifBold24pt7b.h>
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
  Home,
  Hold,
  Check,
  Cycle,
  Sleep,
  DC // hand held is dc from grbl
};

struct refreshVal
{
  std::string x;
  bool isXSet;

  std::string y;
  bool isYSet;

  std::string z;
  bool isZSet;

  std::string a;
  bool isASet;

  std::string feedRate;
  bool isFeedRateSet;

  std::string spindleRate;
  bool isSpindleRateSet;

  std::string progress;
  bool isProgressSet;

  std::string feedRateOV;
  bool isFeedRateOVSET;

  EState state;
  bool isStateSet;

  int jogSpeed = 1000;

  bool alarm;
  bool isMessageShow;
  std::string message;

  std::string macro;

  int memoryMount;
  int stateColor;
};

struct pageProp
{
  int pageCount;
  int currentPage;
  int currentItem;
  bool itemChanged = true;
  std::string name;
  bool isInit;
};

struct sdCard
{
  int freeSpace;
  int usedSpace;
  std::list<std::string> items;
  bool refresh = false;
  bool isMenuCreated = false;
};

static Adafruit_ILI9341 *display = NULL;
void displaySetup(void);
void drawMainTheme(void);
Adafruit_ILI9341 *initDisplay(char rotate);
void nextLcdPage(void);
void setMacroItem(void);
void prevLcdPage(void);
void nextMenuItem(void);
void prevMenuItem(void);
void runItem(void);
void runMacroItem(void);
void goToMainPage(void);
void setDCState(void);
void refresh(void);

#endif // HH_LCD