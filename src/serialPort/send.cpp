#include "send.h"

extern pageProp page;
extern sdCard SD;

void sendTask(void *p)
{
    while (true)
    {
        if (page.currentPage == 1)
        {
            Serial1.write("?");
            vTaskDelay(60);
        }
        else if (page.currentPage == 2 && !SD.refresh && page.isInit)
        {
            Serial1.write("$SD/List");
            Serial1.write("\n");
            vTaskDelay(2000);
        }
        vTaskDelay(5);
    }
}