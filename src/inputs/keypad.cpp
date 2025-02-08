#include "keypad.h"

#include "config.h"
#include "serialPort/recieve.h"

extern refreshVal currentValues;
extern pageProp page;

int sotoon[4] = {9, 12, 11, 10};
int satr[6] = {
    14,
    21,
    47,
    48,
    45,
    16,
};

#ifdef KEYPAD_MATRIX
char *commandMap[6][4] = {
    {"$J=G91 G21 X200 F1000", "$J=G91 G21 Y3000 F1000",
     "$J=G91 G21 Z1000 F1000", "$J=G91 G21 A1000 F1000"},
    {"$J=G91 G21 X-200 F1000", "$J=G91 G21 Y-3000 F1000",
     "$J=G91 G21 Z-1000 F1000", "$J=G91 G21 A-1000 F1000"},
    {"0x91", "0x92", "G10 L20 P0 X0 Y0 Z0 A0", "$H"},
    {"$X", "~", "!", "ctrl-x"},
    {"ok", "fn", "mist", "nextPage"},
    {"spEnable", "sp-", "sp+", "macro"}};
#endif

#ifdef KEYPAD_GB
char *commandMap[6][4] = {
    {"$J=G91 G21 Y3000", "$J=G91 G21 B1000", "$J=G91 G21 Z1000",
     "$J=G91 G21 X3000"},                                                // 0
    {"$J=G91 G21 Z-1000", "$J=G91 G21 Y-3000", "", "$J=G91 G21 X-3000"}, // 1
    {"$J=G91 G21 B-1000", "0x92", "0x91", "ctrl-x"},                     // 2
    {"$X", "~", "$H", "nextPage"},                                       // 3
    {"G10 L20 P0 X0 Y0 Z0 B0", "sp+", "sp-", "!"},                       // 4
    {"spEnable", "mist", "fn", "macro"}                                  // 5
};

char *commandMapSDCard[6][4] = {
    {"Up", "", "", "Right"},    // 0
    {"", "Down", "ok", "left"}, // 1
    {"", "", "", ""},           // 2
    {"", "", "", "nextPage"},   // 3
    {"", "", "", ""},           // 4
    {"", "", "", "macro"}       // 5
};
#endif

void sendCommand(char *command) {}

void keypadTask(void *p)
{
  log_v("Keypad Task Running");
  bool clicked = false;
  unsigned long timeD = 0;
  for (size_t i = 0; i < 6; i++)
    pinMode(satr[i], OUTPUT);
  for (size_t i = 0; i < 4; i++)
    pinMode(sotoon[i], INPUT_PULLUP);

  while (true)
  {
    for (size_t i = 0; i < 6; i++)
    {
      digitalWrite(satr[i], LOW);
      for (size_t j = 0; j < 4; j++)
      {
        clicked = false;
        if (digitalRead(sotoon[j]) == 0)
        {
          clicked = true;
          timeD = millis() + 50;
          while (millis() <= timeD)
          {
            if (digitalRead(sotoon[j]) != 0)
            {
              vTaskDelay(2);
              log_d("debounce");
              clicked = false;
              continue;
            }
          }
          if (clicked)
          {
            if (page.currentPage == 1)
            {
              // doSomeThings onPressed
              log_v("Satr: %i   - Sotoon: %i     (Pressed)", i, j);
              log_d("command: %s", commandMap[i][j]);

              if (isContain(commandMap[i][j], "$J"))
              {
                char buffer[30];
                sprintf(buffer, "%s F%i", commandMap[i][j],
                        currentValues.jogSpeed);
                Serial1.write(buffer);
                Serial1.write("\n");
                while (digitalRead(sotoon[j]) == 0)
                {
                  vTaskDelay(1);
                }

                vTaskDelay(10);
                Serial1.write(0x85);
                Serial1.write("\n");
                vTaskDelay(2);
                Serial1.write("$J=G91 G21 X0.001 F1");
                Serial1.write("\n");
                vTaskDelay(2);
                Serial1.write(0x85);
                Serial1.write("\n");
              }
              else
              {
                if (strcmp(commandMap[i][j], "ctrl-x") == 0)
                {
                  Serial1.write(0x18);
                  Serial1.write("\n");
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "G10 L20 P0 X0 Y0 Z0 B0") ==
                         0)
                {
                  Serial1.write(commandMap[i][j]);
                  Serial1.write("\n");
                  beepBuzzer();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "$X") == 0)
                {
                  Serial1.write(0x18);
                  Serial1.write("\n");
                  vTaskDelay(100);
                  Serial1.write("$X");
                  Serial1.write("\n");
                  clearMessage();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "0x91") == 0)
                {
                  if (currentValues.state == Run)
                  {
                    Serial1.write(0x91);
                    Serial1.write("\n");
                  }
                  else if (currentValues.state == Idle)
                  {
                    if ((currentValues.jogSpeed + (currentValues.jogSpeed * 10 / 100)) >= 10000)
                    {
                      currentValues.jogSpeed = 10000;
                      currentValues.isFeedRateSet = false;
                    }
                    else
                    {
                      if (currentValues.jogSpeed < 10000)
                      {
                        currentValues.jogSpeed =
                            currentValues.jogSpeed +
                            (currentValues.jogSpeed * 10 / 100);
                        currentValues.isFeedRateSet = false;
                      }
                    }
                  }
                  beepBuzzer();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "0x92") == 0)
                {
                  if (currentValues.state == Run)
                  {
                    Serial1.write(0x92);
                    Serial1.write("\n");
                  }
                  else if (currentValues.state == Idle)
                  {
                    if (currentValues.jogSpeed > 30)
                    {
                      currentValues.jogSpeed =
                          currentValues.jogSpeed -
                          (currentValues.jogSpeed * 10 / 100);
                      currentValues.isFeedRateSet = false;
                    }
                  }
                  beepBuzzer();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "nextPage") == 0)
                {
                  nextLcdPage();
                  beepBuzzer();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "macro") == 0)
                {
                  Serial1.write("G10 L20 P0 X0 Y0 Z0 A0");
                  Serial1.write("\n");
                  runMacroItem();
                  beepBuzzer();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "mist") == 0)
                {
                  Serial1.write("M7");
                  Serial1.write("\n");
                  beepBuzzer();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "fn") == 0)
                {
                  Serial1.write("M9");
                  Serial1.write("\n");
                  beepBuzzer();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "sp-") == 0)
                {
                  int isEnable = currentValues.spindleRate.compare("0");
                  if (isEnable)
                  {
                    Serial1.write(0x9A);
                    Serial1.write("\n");
                    beepBuzzer();
                  }
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "sp+") == 0)
                {
                  int isEnable = currentValues.spindleRate.compare("0");
                  if (isEnable)
                  {
                    Serial1.write(0x9B);
                    Serial1.write("\n");
                    beepBuzzer();
                  }
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else if (strcmp(commandMap[i][j], "spEnable") == 0)
                {
                  int isEnable = currentValues.spindleRate.compare("0");
                  if (!isEnable)
                  {
                    Serial1.write("M3 S150");
                    Serial1.write("\n");
                  }
                  else
                  {
                    Serial1.write(0x99);
                    Serial1.write("\n");
                    Serial1.write("M5");
                    Serial1.write("\n");
                  }
                  beepBuzzer();
                  while (digitalRead(sotoon[j]) == 0)
                  {
                    vTaskDelay(1);
                  }
                  continue;
                }
                else
                {
                  Serial1.write(commandMap[i][j]);
                  Serial1.write("\n");
                  vTaskDelay(3);
                }
                while (digitalRead(sotoon[j]) == 0)
                {
                  vTaskDelay(1);
                }
              }

              // doSomeThings onDisPressed
              log_v("Satr: %i   - Sotoon: %i     (DisPressed)", i, j);
              log_v();
            }
            else if (page.currentPage == 2)
            {
              if (strcmp(commandMapSDCard[i][j], "nextPage") == 0)
              {
                nextLcdPage();
                beepBuzzer();
                while (digitalRead(sotoon[j]) == 0)
                {
                  vTaskDelay(1);
                }
                continue;
              }
              else if (strcmp(commandMapSDCard[i][j], "ok") == 0)
              {
                Serial1.write("G10 L20 P0 X0 Y0 Z0 A0");
                Serial1.write("\n");
                beepBuzzer();
                runItem();
                while (digitalRead(sotoon[j]) == 0)
                {
                  vTaskDelay(1);
                }
                continue;
              }
              else if (strcmp(commandMapSDCard[i][j], "Down") == 0)
              {
                nextMenuItem();
                beepBuzzer();
                while (digitalRead(sotoon[j]) == 0)
                {
                  vTaskDelay(1);
                }
                continue;
              }
              else if (strcmp(commandMapSDCard[i][j], "Up") == 0)
              {
                prevMenuItem();
                beepBuzzer();
                while (digitalRead(sotoon[j]) == 0)
                {
                  vTaskDelay(1);
                }
                continue;
              }
              else if (strcmp(commandMapSDCard[i][j], "macro") == 0)
              {
                setMacroItem();
                beepBuzzer();
                while (digitalRead(sotoon[j]) == 0)
                {
                  vTaskDelay(1);
                }
                continue;
              }
            }
          }
        }
      }
      digitalWrite(satr[i], HIGH);
    }
    vTaskDelay(5);
  }
}

void keypadSetup(void)
{
  xTaskCreate(keypadTask, "keypadTask", 10 * 1024, NULL, 600, NULL);
}