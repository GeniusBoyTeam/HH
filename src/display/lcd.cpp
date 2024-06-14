#include "lcd.h"

refreshVal currentValues;
refreshVal lastValues;
pageProp page;
sdCard SD;

char *enumStates[10] = {"Idle", "Run", "Jog", "Alarm", "Door", "Home", "Hold", "Check", "Cycle", "Sleep"};

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
  display->fillScreen(ILI9341_BLACK);
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

void drawSDTheme(void)
{
  display->fillScreen(ILI9341_BLACK);
  display->fillRoundRect(10, -10, 300, 55, 10, ILI9341_YELLOW);
  display->setTextColor(ILI9341_BLACK);
  display->setTextSize(1);
  display->setCursor(53, 32);
  display->setFont(&FreeSerifBold18pt7b);
  display->print("GRBL Storage");
}

void nextLcdPage()
{
  SD.refresh = false;
  SD.isMenuCreated = false;
  int resultPage = page.currentPage + 1;
  if (resultPage > page.pageCount)
  {
    resultPage = 1;
  }
  page.currentPage = resultPage;
  log_v("CurrentPage= %i", page.currentPage);
  page.isInit = false;
  page.itemChanged = true;
}

void goToMainPage()
{
  SD.refresh = false;
  SD.isMenuCreated = false;
  page.currentPage = 1;
  page.isInit = false;
  page.itemChanged = true;
}

void nextMenuItem()
{
  int resultItem = page.currentItem + 1;
  if (resultItem > SD.items.size() - 1)
  {
    resultItem = 0;
  }
  page.currentItem = resultItem;
  page.itemChanged = true;
}

void prevMenuItem()
{
  int resultItem = page.currentItem - 1;
  if (resultItem == -1)
  {
    resultItem = SD.items.size() - 1;
  }
  page.currentItem = resultItem;
  page.itemChanged = true;
}

void prevLcdPage()
{
  page.itemChanged = true;
  int resultPage = page.currentPage - 1;
  if (resultPage == 0)
  {
    resultPage = page.pageCount;
  }
  page.currentPage = resultPage;
  log_v("CurrentPage= %i", page.currentPage);
  page.isInit = false;
}

void runItem()
{
  char buffer[100];
  auto strItem = SD.items.begin();
  std::advance(strItem, page.currentItem);
  sprintf(buffer, "$SD/RUN=%s", strItem->c_str());
  Serial1.write(buffer);
  Serial1.write("\n");
  goToMainPage();
}

int startX = 60;
void refreshXPos(void)
{
  int x = currentValues.x.compare(lastValues.x);
  if (x != 0)
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
}

void refreshYPos(void)
{
  int y = currentValues.y.compare(lastValues.y);
  if (y != 0)
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
}

void refreshZPos(void)
{
  int z = currentValues.z.compare(lastValues.z);
  if (z != 0)
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
}

void refreshAPos(void)
{
  int a = currentValues.a.compare(lastValues.a);
  if (a != 0)
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
}

void refreshState(void)
{
  if (!currentValues.isStateSet)
  {
    log_v("Refresh State: %i", currentValues.state);
    currentValues.isStateSet = true;
    display->fillRoundRect(240, 195, 75, 30, 5, currentValues.stateColor);
    display->setFont(&FreeSerifBold9pt7b);
    display->setCursor(250, 215);
    display->setTextSize(1);
    display->setTextColor(ILI9341_BLACK);
    display->print(enumStates[currentValues.state]);
  }
}

void refresh(void)
{
  log_v("Refresh Main Display");

  refreshXPos();
  refreshYPos();
  refreshZPos();
  refreshAPos();
  refreshState();
}

void createMenuItems(void)
{
  int start = 75;
  int gap = 20;
  int counter = 0;
  for (std::string item : SD.items)
  {
    display->setTextColor(ILI9341_WHITE);
    display->setFont(&FreeSerifBold9pt7b);
    display->setTextSize(1);
    display->setCursor(40, start + (gap * counter));
    display->print(item.c_str());
    counter++;
  }
}

void refreshMenuCurser()
{
  int start = 66;
  int gap = 20;
  int counter = 0;
  display->fillRect(17, 60, 20, 130, ILI9341_BLACK);
  display->fillRoundRect(17, start + (gap * page.currentItem), 18, 5, 2, ILI9341_YELLOW);
}

void displayTask(void *p)
{
  initDisplay(3);
  page.currentPage = 1;
  page.pageCount = 2;
  while (true)
  {
    switch (page.currentPage)
    {
    case 1: // main page
    {
      if (!page.isInit)
      {
        drawMainTheme();
        page.isInit = true;
      }
      refresh();
      vTaskDelay(80);
      break;
    }
    case 2: // SD page
    {
      if (!page.isInit)
      {
        drawSDTheme();
        SD.items.clear();
        page.isInit = true;
      }
      if (!SD.isMenuCreated)
      {
        if (SD.refresh)
        {
          SD.isMenuCreated = true;
          page.currentItem = 0;
          createMenuItems();
        }
      }
      if (page.itemChanged)
      {
        page.itemChanged = false;
        refreshMenuCurser();
      }

      vTaskDelay(50);
      break;
    }
    default:
      break;
    }
  }
}

void displaySetup(void)
{
  xTaskCreate(displayTask, "displayTask", 10 * 1024, NULL, 500, NULL);
}