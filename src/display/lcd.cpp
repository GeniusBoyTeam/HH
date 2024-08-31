#include "lcd.h"

refreshVal currentValues;
refreshVal lastValues;
pageProp page;
sdCard SDCard;

char *enumStates[11] = {"Idle", "Run", "Jog", "Alarm", "Door",
                        "Home", "Hold", "Check", "Cycle", "Sleep", "DC"};

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

void setDCState(void)
{
  currentValues.state = DC;
}

void overWritePosition(EChar character, double value) {}

int16_t convertLocation(int start, int gap, int componentHeight, int number)
{
  return start + (gap * number) + (componentHeight * number);
}

void drawMainTheme(void)
{
  display->fillScreen(ILI9341_BLACK);
  int gap = 1;
  int start = 45;
  int componentHeght = 47;
  int startRects = 6;
  int roundRects = 5;

  display->setTextColor(ILI9341_BLACK);
  display->setTextSize(1);
  display->setFont(&FreeSerifBold24pt7b);
  display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 0),
                         componentHeght, componentHeght, roundRects,
                         ILI9341_GREENYELLOW);
  display->setCursor(12, convertLocation(start, gap, componentHeght, 0));
  display->print("X");
  display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 1),
                         componentHeght, componentHeght, roundRects,
                         ILI9341_GREENYELLOW);
  display->setCursor(12, convertLocation(start, gap, componentHeght, 1));
  display->print("Y");
  display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 2),
                         componentHeght, componentHeght, roundRects,
                         ILI9341_GREENYELLOW);
  display->setCursor(12, convertLocation(start, gap, componentHeght, 2));
  display->print("Z");
  display->fillRoundRect(5, convertLocation(startRects, gap, componentHeght, 3),
                         componentHeght, componentHeght, roundRects,
                         ILI9341_GREENYELLOW);
  display->setCursor(12, convertLocation(start, gap, componentHeght, 3));
  display->print("A");

  currentValues.isXSet = false;
  currentValues.isYSet = false;
  currentValues.isZSet = false;
  currentValues.isASet = false;
  currentValues.isSpindleRateSet = false;
  currentValues.isFeedRateSet = false;
  currentValues.isStateSet = false;
  currentValues.isFeedRateOVSET = false;
  currentValues.isProgressSet = false;
  SDCard.refresh = false;

  display->setTextSize(1);
  display->setFont(&FreeSerifBold9pt7b);

  display->fillRoundRect(200, 5, 115, 62, 5, ILI9341_GREENYELLOW);
  display->setTextColor(ILI9341_BLACK);
  display->setRotation(2);
  display->setCursor(192, 225);
  display->print("SD");
  display->setRotation(3);
  display->fillRoundRect(240, 9, 70, 25, 4, ILI9341_BLACK);
  display->setFont(&FreeSerifBold9pt7b);
  display->setTextColor(ILI9341_WHITE);
  display->setTextSize(1);
  display->setCursor(248, 26);
  display->print("p:null");
  display->fillRoundRect(240, 38, 70, 25, 4, ILI9341_BLACK);
  display->setCursor(248, 55);
  display->print("f:100%");
  // sd

  display->fillRoundRect(200, 72, 115, 62, 5, ILI9341_GREENYELLOW);
  display->setTextColor(ILI9341_BLACK);
  display->setRotation(2);
  display->setCursor(108, 225);
  display->print("Spindle");
  display->setRotation(3);
  display->fillRoundRect(240, 88, 70, 30, 4, ILI9341_BLACK);
  display->setFont(&FreeSerifBold9pt7b);
  display->setTextColor(ILI9341_WHITE);
  display->setTextSize(1);
  display->setCursor(248, 108);
  display->print("0");
  // spindle

  display->fillRoundRect(200, 139, 115, 62, 5, ILI9341_GREENYELLOW);
  display->setTextColor(ILI9341_BLACK);
  display->setRotation(2);
  display->setCursor(53, 215);
  display->print("Feed");
  display->setCursor(53, 230);
  display->print("Rate");
  display->setRotation(3);
  display->fillRoundRect(240, 143, 70, 25, 4, ILI9341_BLACK);
  display->fillRoundRect(240, 172, 70, 25, 4, ILI9341_BLACK);
  display->setFont(&FreeSerifBold9pt7b);
  display->setTextColor(ILI9341_WHITE);
  display->setTextSize(1);
  display->setCursor(248, 163);
  display->print("0");
  display->setCursor(248, 192);
  display->print("0");
  // feedRate

  display->fillRoundRect(5, 205, 65, 25, 5, ILI9341_WHITE);
  lastValues.state = DC;
  // state

  display->fillRoundRect(73, 205, 242, 25, 5, ILI9341_DARKGREY);
  // message

  display->drawLine(195, 0, 195, 200, ILI9341_WHITE);
  display->drawLine(0, 200, 195, 200, ILI9341_WHITE);
}

void refreshMacroVal(void)
{
  display->fillRoundRect(105, 30, 180, 20, 10, ILI9341_GREENYELLOW);
  display->setTextColor(ILI9341_BLACK);
  display->setTextSize(1);
  display->setCursor(108, 45);
  display->setFont(&FreeSerifBold9pt7b);
  display->print(currentValues.macro.c_str());
}

void drawSDTheme(void)
{
  display->fillScreen(ILI9341_BLACK);
  display->fillRoundRect(10, -28, 300, 55, 10, ILI9341_GREENYELLOW);
  display->fillRoundRect(35, 30, 250, 20, 10, ILI9341_GREENYELLOW);
  display->setTextColor(ILI9341_BLACK);
  display->setTextSize(1);
  display->setCursor(85, 18);
  display->setFont(&FreeSerifBold12pt7b);
  display->print("GRBL Storage");

  display->fillRect(17, 60, 20, 130, ILI9341_WHITE);
  display->setTextColor(ILI9341_BLACK);
  display->setTextSize(1);
  display->setCursor(40, 45);
  display->setFont(&FreeSerifBold9pt7b);
  display->print("Macro :");

  refreshMacroVal();
}

void nextLcdPage()
{
  SDCard.refresh = false;
  SDCard.isMenuCreated = false;
  int resultPage = page.currentPage + 1;
  if (resultPage > page.pageCount)
  {
    resultPage = 1;
  }
  page.currentPage = resultPage;
  log_v("CurrentPage= %i", page.currentPage);
  page.isInit = false;
  page.currentItem = 0;
  page.itemChanged = true;
}

void goToMainPage()
{
  SDCard.refresh = false;
  SDCard.isMenuCreated = false;
  page.currentPage = 1;
  page.isInit = false;
  page.currentItem = 0;
  page.itemChanged = true;
}

void nextMenuItem()
{
  int resultItem = page.currentItem + 1;
  if (resultItem > SDCard.items.size() - 1)
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
    resultItem = SDCard.items.size() - 1;
  }
  page.currentItem = resultItem;
  page.itemChanged = true;
}

void setMacroItem()
{
  char buffer[100];
  auto strItem = SDCard.items.begin();
  std::advance(strItem, page.currentItem);
  sprintf(buffer, "%s", strItem->c_str());
  currentValues.macro = buffer;

  refreshMacroVal();
}

void prevLcdPage()
{
  page.currentItem = 0;
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
  auto strItem = SDCard.items.begin();
  std::advance(strItem, page.currentItem);
  sprintf(buffer, "$SD/RUN=%s", strItem->c_str());
  showMessage(strItem->c_str());
  Serial1.write(buffer);
  Serial1.write("\n");
  goToMainPage();
}

void runMacroItem()
{
  char buffer[100];
  sprintf(buffer, "$SD/RUN=%s", currentValues.macro.c_str());
  Serial1.write(buffer);
  Serial1.write("\n");
  goToMainPage();
}

int startX = 60;
void refreshXPos(void)
{
  if (!currentValues.isXSet)
  {
    log_v("Refresh XPos");
    currentValues.isXSet = true;

    int gap = 1;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->fillRoundRect(startX, convertLocation(15, gap, componentHeght, 0),
                           130, 32, 5, ILI9341_BLACK);
    display->setCursor(startX,
                       convertLocation(start - 4, gap, componentHeght, 0));
    display->setTextColor(ILI9341_WHITE);
    display->print(currentValues.x.c_str());
  }
}

void refreshYPos(void)
{
  if (!currentValues.isYSet)
  {
    log_v("Refresh YPos");
    currentValues.isYSet = true;

    int gap = 1;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->fillRoundRect(startX, convertLocation(15, gap, componentHeght, 1),
                           130, 32, 5, ILI9341_BLACK);
    display->setCursor(startX,
                       convertLocation(start - 4, gap, componentHeght, 1));
    display->setTextColor(ILI9341_WHITE);
    display->print(currentValues.y.c_str());
  }
}

void refreshZPos(void)
{
  if (!currentValues.isZSet)
  {
    log_v("Refresh ZPos");
    currentValues.isZSet = true;

    int gap = 1;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->fillRoundRect(startX, convertLocation(15, gap, componentHeght, 2),
                           130, 32, 5, ILI9341_BLACK);
    display->setCursor(startX,
                       convertLocation(start - 4, gap, componentHeght, 2));
    display->setTextColor(ILI9341_WHITE);
    display->print(currentValues.z.c_str());
  }
}

void refreshAPos(void)
{
  if (!currentValues.isASet)
  {
    log_v("Refresh APos");
    currentValues.isASet = true;

    int gap = 1;
    int start = 45;
    int componentHeght = 47;
    int startRects = 6;
    int roundRects = 5;
    display->setTextSize(1);
    display->setFont(&FreeSerifBold18pt7b);
    display->fillRoundRect(startX, convertLocation(15, gap, componentHeght, 3),
                           130, 32, 5, ILI9341_BLACK);
    display->setCursor(startX,
                       convertLocation(start - 4, gap, componentHeght, 3));
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
    display->fillRoundRect(5, 205, 65, 25, 5, currentValues.stateColor);
    display->setFont(&FreeSerifBold9pt7b);
    display->setTextSize(1);
    display->setTextColor(ILI9341_BLACK);
    display->setCursor(10, 223);
    display->print(enumStates[currentValues.state]);
  }
}

void refreshMesage(void)
{
  if (!currentValues.isMessageShow)
  {
    log_i("Refresh Message: %s", currentValues.message);
    currentValues.isMessageShow = true;
    display->fillRoundRect(73, 205, 242, 25, 5, ILI9341_DARKGREY);
    display->setFont(&FreeSerifBold9pt7b);
    display->setTextSize(1);
    display->setTextColor(ILI9341_RED);
    display->setCursor(78, 223);
    display->print(currentValues.message.c_str());
  }
}

void refreshFeedRate(void)
{
  if (!currentValues.isFeedRateSet)
  {
    currentValues.isFeedRateSet = true;
    display->fillRoundRect(240, 143, 70, 25, 4, ILI9341_BLACK);
    display->fillRoundRect(240, 172, 70, 25, 4, ILI9341_BLACK);
    display->setFont(&FreeSerifBold9pt7b);
    display->setTextColor(ILI9341_WHITE);
    display->setTextSize(1);
    display->setCursor(248, 163);
    display->print(currentValues.feedRate.c_str());
    display->setCursor(248, 192);
    display->print(((String)(currentValues.jogSpeed)).c_str());
  }
}

void refreshSpindleRate(void)
{
  if (!currentValues.isSpindleRateSet)
  {
    currentValues.isSpindleRateSet = true;
    display->fillRoundRect(240, 88, 70, 30, 4, ILI9341_BLACK);
    display->setFont(&FreeSerifBold9pt7b);
    display->setTextColor(ILI9341_WHITE);
    display->setTextSize(1);
    display->setCursor(248, 108);
    display->print(currentValues.spindleRate.c_str());
  }
}

void refreshProgress(void)
{
  if (!currentValues.isProgressSet)
  {
    currentValues.isProgressSet = true;
    display->fillRoundRect(240, 9, 70, 25, 4, ILI9341_BLACK);
    display->setFont(&FreeSerifBold9pt7b);
    display->setTextColor(ILI9341_WHITE);
    display->setTextSize(1);
    display->setCursor(248, 26);

    String newVal = String(currentValues.progress.c_str());
    double temp = round(newVal.toDouble() * 100.0) / 100.0;
    char buffer[10];
    sprintf(buffer, "p:%.0lf%%", temp);
    display->print(buffer);
  }
}

void refreshOV(void)
{
  if (!currentValues.isFeedRateOVSET)
  {
    currentValues.isFeedRateOVSET = true;
    display->setFont(&FreeSerifBold9pt7b);
    display->setTextColor(ILI9341_WHITE);
    display->setTextSize(1);
    display->setCursor(248, 55);
    display->fillRoundRect(240, 38, 70, 25, 4, ILI9341_BLACK);
    char buffer[10];
    sprintf(buffer, "o:%s%%", currentValues.feedRateOV.c_str());
    display->print(buffer);
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
  refreshMesage();
  refreshFeedRate();
  refreshSpindleRate();
  refreshOV();
  if (currentValues.state == Run)
  {
    refreshProgress();
  }
}

void createMenuItems(void)
{
  int start = 75;
  int gap = 20;
  int counter = 0;
  for (std::string item : SDCard.items)
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
  display->fillRoundRect(17, start + (gap * page.currentItem), 18, 5, 2,
                         ILI9341_GREENYELLOW);
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
        SDCard.items.clear();
        page.isInit = true;
      }
      if (!SDCard.isMenuCreated)
      {
        if (SDCard.refresh)
        {
          SDCard.isMenuCreated = true;
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