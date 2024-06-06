#include "recieve.h"

using namespace std;

extern refreshVal currentValues;
extern refreshVal lastValues;

list<string> splitString(string input, string splitChar)
{
    list<string> splitedList;

    for (char *token = strtok(const_cast<char *>(input.data()), splitChar.c_str()); token != nullptr; token = strtok(nullptr, splitChar.c_str()))
    {
        splitedList.push_back(token);
    }
    return splitedList;
}

string getString(char *str, int len)
{
    return string(str, len);
}

bool isContain(string input, char *findingChar)
{
    string s = input;
    if (s.rfind(findingChar, 0) == 0)
        return true;
    return false;
}

// Remove first and last char of seting
string cutExtraChar(string input)
{
    std::string s = input;
    s.erase(0, 1);
    s.erase(s.size() - 2);
    return s;
}

void setState(string state)
{
    string stateNew = state;
    if (isContain(stateNew, ":"))
    {
        stateNew = splitString(stateNew, ":").front();
    }

    if (strcmp(stateNew.c_str(), "Idle") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Idle;
    }
    else if (strcmp(stateNew.c_str(), "Run") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Run;
    }
    else if (strcmp(stateNew.c_str(), "Jog") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Jog;
    }
    else if (strcmp(stateNew.c_str(), "Alarm") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Alarm;
    }
    else if (strcmp(stateNew.c_str(), "Door") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Door;
    }
    else if (strcmp(stateNew.c_str(), "Homing") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Homing;
    }
    else if (strcmp(stateNew.c_str(), "Hold") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Hold;
    }
    else if (strcmp(stateNew.c_str(), "Check") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Check;
    }
    else if (strcmp(stateNew.c_str(), "Cycle") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Cycle;
    }
    else if (strcmp(stateNew.c_str(), "Sleep") == 0)
    {
        lastValues.state = currentValues.state;
        currentValues.state = Sleep;
    }
}

void setPositions(string MPos)
{
    lastValues.x = currentValues.x;
    lastValues.y = currentValues.y;
    lastValues.z = currentValues.z;
    lastValues.a = currentValues.a;

    list<string> positionsList = splitString(MPos.c_str(), ",");
    int counter = 0;

    while (positionsList.size() != 0)
    {
        switch (counter)
        {
        case 0:
        {
            currentValues.x = (positionsList.front()).c_str();
            log_v("X_CURRENT: %s", currentValues.x.c_str());
            log_v("X_LAST: %s", lastValues.x.c_str());
            break;
        }
        case 1:
        {
            currentValues.y = (positionsList.front()).c_str();
            log_v("Y_CURRENT: %s", currentValues.y.c_str());
            break;
        }
        case 2:
        {
            currentValues.z = (positionsList.front()).c_str();
            log_v("Z_CURRENT: %s", currentValues.z.c_str());
            break;
        }
        case 3:
        {
            currentValues.a = (positionsList.front()).c_str();
            log_v("A_CURRENT: %s", currentValues.a.c_str());
            break;
        }
        default:
            break;
        }
        positionsList.pop_front();
        counter++;
    }
}

void parseRecieved(string data)
{
    // example input
    //<Door:0|MPos:0.000,0.000,0.000,0.000|FS:0,0|Pn:P|WCO:0.000,0.000,0.000,0.000>

    if (isContain(data.c_str(), "<")) // response for (?)
    {
        log_v("RECIEVED: %s", data.c_str());
        list<string> parsedData = splitString(cutExtraChar(data.c_str()).c_str(), "|");

        setState(parsedData.front());
        parsedData.pop_front();

        for (string splited : parsedData)
        {
            if (isContain(splited, "MPos") || isContain(splited, "WPos"))
            {
                setPositions((splited.erase(0, 5)).c_str());
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
                parseRecieved(getString(receivedChars, ndx));
                //..................................
                receivedChars[ndx] = '\0';
                ndx = 0;
                newData = true;
            }
            newData = false;
        }
        vTaskDelay(50);
    }
}