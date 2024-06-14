#include "keypad.h"

int sotoon[4] = {9, 12, 11, 10};
int satr[6] = {
    14,
    21,
    47,
    48,
    45,
    13,
};

char *commandMap[5][4] = {
    {"$J=G91 G21 X200 F1000", "$J=G91 G21 Y3000 F1000", "$J=G91 G21 Z1000 F1000", "$J=G91 G21 A1000 F1000"},
    {"$J=G91 G21 X-200 F1000", "$J=G91 G21 Y-3000 F1000", "$J=G91 G21 Z-1000 F1000", "$J=G91 G21 A-1000 F1000"},
    {"0x91", "0x92", "G10 L20 P0 X0 Y0 Z0 A0", "$H"},
    {"$X", "~", "!", "ctrl-x"},
    {"ok", "prevRow", "nextRow", "nextPage"},
};

void keypadTask(void *p)
{
    log_v("Keypad Task Running");

    for (size_t i = 0; i < 6; i++)
        pinMode(satr[i], OUTPUT);
    for (size_t i = 0; i < 4; i++)
        pinMode(sotoon[i], INPUT);

    while (true)
    {
        for (size_t i = 0; i < 6; i++)
        {
            digitalWrite(satr[i], LOW);
            for (size_t j = 0; j < 4; j++)
            {
                if (digitalRead(sotoon[j]) == 0)
                {
                    // doSomeThings onPressed
                    log_v("Satr: %i   - Sotoon: %i     (Pressed)", i, j);
                    if (strcmp(commandMap[i][j], "ctrl-x") == 0)
                    {
                        Serial1.write(0x18);
                        Serial1.write("\n");
                    }
                    else if (strcmp(commandMap[i][j], "0x91") == 0)
                    {
                        Serial1.write(0x91);
                        Serial1.write("\n");
                    }
                    else if (strcmp(commandMap[i][j], "0x92") == 0)
                    {
                        Serial1.write(0x92);
                        Serial1.write("\n");
                    }
                    else if (strcmp(commandMap[i][j], "nextPage") == 0)
                    {
                        nextLcdPage();
                        while (digitalRead(sotoon[j]) == 0)
                        {
                            vTaskDelay(1);
                        }
                        continue;
                    }
                    else if (strcmp(commandMap[i][j], "nextRow") == 0)
                    {
                        nextMenuItem();
                        while (digitalRead(sotoon[j]) == 0)
                        {
                            vTaskDelay(1);
                        }
                        continue;
                    }
                    else if (strcmp(commandMap[i][j], "prevRow") == 0)
                    {
                        prevMenuItem();
                        while (digitalRead(sotoon[j]) == 0)
                        {
                            vTaskDelay(1);
                        }
                        continue;
                    }
                    else if (strcmp(commandMap[i][j], "ok") == 0)
                    {
                        runItem();
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

                    log_v("COMMAND: [ %s ]", commandMap[i][j]);
                    while (digitalRead(sotoon[j]) == 0)
                    {
                        vTaskDelay(1);
                    }
                    if (i <= 2)
                    {
                        vTaskDelay(1);
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

void keypadSetup(void)
{
    xTaskCreate(keypadTask, "keypadTask", 10 * 1024, NULL, 200, NULL);
}