#include "send.h"

extern pageProp page;
extern sdCard SDCard;
extern refreshVal currentValues;

void sendTask(void *p)
{
    while (true)
    {
        if (page.currentPage == 1)
        {
            int delay = (currentValues.state == Run) ? 300 : 130;
            Serial1.write("?");
            vTaskDelay(delay);
        }
        else if (page.currentPage == 2 && !SDCard.refresh && page.isInit)
        {
            Serial1.write("$SD/List");
            Serial1.write("\n");
            vTaskDelay(2000);
        }
        vTaskDelay(5);
    }
}