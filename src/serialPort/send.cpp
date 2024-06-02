#include "send.h"

void sendTask(void *p)
{
    while (true)
    {
        Serial1.write("?");
        vTaskDelay(60);
    }
}