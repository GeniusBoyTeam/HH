#include "recieve.h"

void parseRecieved(char *data)
{
    log_i("RECIEVED: %s", data);
}

void recieveTask(void *p)
{
    const byte numChars = 150;
    char receivedChars[numChars];
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    boolean newData = false;

    while (true)
    {
        while (Serial1.available() > 0 && newData == false)
        {
            rc = Serial1.read();

            if (rc != endMarker)
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars)
                {
                    ndx = numChars - 1;
                }
            }
            else
            {
                // Do something with recieved data...
                parseRecieved(receivedChars);
                //..................................
                receivedChars[ndx] = '\0';
                ndx = 0;
                newData = true;
            }
        }
        newData = false;
        vTaskDelay(25);
    }
}