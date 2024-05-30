#include "send.h"

void sendTask(void *p)
{
    while (true)
    {
        Serial1.write("?");
        Serial1.write("\n");
        vTaskDelay(30);
    }
}