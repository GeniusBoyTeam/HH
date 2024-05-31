#include "recieve.h"

std::list<char *> splitString(char *input, char *splitChar)
{
    std::list<char *> splitedList;
    char *chars_array = strtok(input, splitChar);
    while (chars_array != NULL)
    {
        splitedList.push_back(chars_array);
        chars_array = strtok(NULL, splitChar);
    }
    log_v("Splited count: %zu", splitedList.size());
    return splitedList;
}

bool isContain(char *input, char *findingChar)
{
    std::string s = input;
    if (s.rfind(findingChar, 0) == 0)
        return true;
    return false;
}

const char *cutExtraChar(char *input)
{
    std::string s = input;
    s.erase(0, 1);
    s.erase(s.size() - 1);
    return s.c_str();
}

void parseRecieved(char *data)
{
    // example inout
    //<Door:0|MPos:0.000,0.000,0.000,0.000|FS:0,0|Pn:P|WCO:0.000,0.000,0.000,0.000>

    // log_i("RECIEVED: %s", data);
    if (isContain(data, "<")) // response for (?)
    {
        std::list<char *> parsedData = splitString(data, "|");
        for (char *splited : parsedData)
        {
            if (isContain(splited, "MPos"))
            {
                std::string mamad = splited;
                mamad.erase(0,5);
                log_i("MPos ->  %s", mamad.c_str());

            }
            else if (isContain(splited, "FS"))
            {
                log_v("FS: %s", splited);
            }
            else if (isContain(splited, "Pn"))
            {
                log_v("Pn: %s", splited);
            }
            else if (isContain(splited, "WCO"))
            {
                log_v("WCO: %s", splited);
            }
            log_v("Splited recieve: %s", splited);
        }
    }
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
        vTaskDelay(10);
    }
}