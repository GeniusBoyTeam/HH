#include "keypad.h"

#include "config.h"

extern refreshVal currentValues;

int sotoon[4] = {9, 12, 11, 10};
int satr[6] = {
    14, 21, 47, 48, 45, 16,
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
    {"$J=G91 G21 Y3000 F1000", "$J=G91 G21 A1000 F1000",
     "$J=G91 G21 Z1000 F1000", "$J=G91 G21 X200 F1000"},  // 0
    {"$J=G91 G21 Z-1000 F1000", "$J=G91 G21 Y-3000 F1000", "ok",
     "$J=G91 G21 X-200 F1000"},                         // 1
    {"$J=G91 G21 A-1000 F1000", "0x92", "0x91", "ctrl-x"},  // 2
    {"$X", "~", "$H", "nextPage"},                  // 3
    {"G10 L20 P0 X0 Y0 Z0 A0", "sp+", "sp-", "!"},      // 4
    {"spEnable", "mist", "fn", "macro"}                 // 5
};
#endif

void keypadTask(void *p) {
  log_v("Keypad Task Running");

  for (size_t i = 0; i < 6; i++) pinMode(satr[i], OUTPUT);
  for (size_t i = 0; i < 4; i++) pinMode(sotoon[i], INPUT);

  while (true) {
    for (size_t i = 0; i < 6; i++) {
      digitalWrite(satr[i], LOW);
      for (size_t j = 0; j < 4; j++) {
        if (digitalRead(sotoon[j]) == 0) {
          // doSomeThings onPressed
          log_v("Satr: %i   - Sotoon: %i     (Pressed)", i, j);
          log_v("command: %s", commandMap[i][j]);
          if (strcmp(commandMap[i][j], "ctrl-x") == 0) {
            Serial1.write(0x18);
            Serial1.write("\n");
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "G10 L20 P0 X0 Y0 Z0 A0") == 0) {
            Serial1.write(commandMap[i][j]);
            Serial1.write("\n");
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "$X") == 0) {
            Serial1.write(0x18);
            Serial1.write("\n");
            vTaskDelay(100);
            Serial1.write("$X");
            Serial1.write("\n");
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "0x91") == 0) {
            Serial1.write(0x91);
            Serial1.write("\n");
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "0x92") == 0) {
            Serial1.write(0x92);
            Serial1.write("\n");
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "nextPage") == 0) {
            nextLcdPage();
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "sp+") == 0) {
            Serial1.write(0x9A);
            Serial1.write("\n");
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "sp-") == 0) {
            Serial1.write(0x9B);
            Serial1.write("\n");
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "nextRow") == 0) {
            nextMenuItem();
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "ok") == 0) {
            runItem();
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else if (strcmp(commandMap[i][j], "spEnable") == 0) {
            int isEnable = currentValues.spindleRate.compare("0");
            if (!isEnable) {
              Serial1.write("M3 S150");
              Serial1.write("\n");
            } else {
              Serial1.write("M5");
              Serial1.write("\n");
            }
            beepBuzzer();
            while (digitalRead(sotoon[j]) == 0) {
              vTaskDelay(1);
            }
            continue;
          } else {
            Serial1.write(commandMap[i][j]);
            Serial1.write("\n");
            vTaskDelay(3);
          }

          log_v("COMMAND: [ %s ]", commandMap[i][j]);
          while (digitalRead(sotoon[j]) == 0) {
            vTaskDelay(1);
          }
          if (i <= 2) {
            Serial1.write(133);
            // Serial1.write("\n");
            // Serial1.write(0x85);
          }
          // doSomeThings onDisPressed
          log_v("Satr: %i   - Sotoon: %i     (DisPressed)", i, j);
          log_v();
        }
      }
      digitalWrite(satr[i], HIGH);
    }
    vTaskDelay(5);
  }
}

void keypadSetup(void) {
  xTaskCreate(keypadTask, "keypadTask", 10 * 1024, NULL, 200, NULL);
}