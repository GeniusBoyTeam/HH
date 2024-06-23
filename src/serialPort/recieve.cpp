#include "recieve.h"

using namespace std;

extern refreshVal currentValues;
extern refreshVal lastValues;
extern pageProp page;
extern sdCard SD;

list<string> splitString(string input, string splitChar) {
  list<string> splitedList;

  for (char *token =
           strtok(const_cast<char *>(input.data()), splitChar.c_str());
       token != nullptr; token = strtok(nullptr, splitChar.c_str())) {
    splitedList.push_back(token);
  }
  return splitedList;
}

string getString(char *str, int len) { return string(str, len); }

bool isContain(string input, string findingChar) {
  string s = input.c_str();
  if (s.find(findingChar) != string::npos) return true;
  return false;
}

// Remove first and last char of seting
string cutExtraChar(string input) {
  std::string s = input;
  s.erase(0, 1);
  s.erase(s.size() - 2);
  return s;
}

void setState(string state) {
  log_v("state: %s", state.c_str());
  string stateNew = state;
  lastValues.state = currentValues.state;
  if (isContain(state, ":")) stateNew = splitString(state.c_str(), ":").front();

  if (strcmp(stateNew.c_str(), "Idle") == 0) {
    currentValues.state = Idle;
    currentValues.stateColor = ILI9341_WHITE;
  } else if (strcmp(stateNew.c_str(), "Run") == 0) {
    currentValues.state = Run;
    currentValues.stateColor = ILI9341_YELLOW;
  } else if (strcmp(stateNew.c_str(), "Jog") == 0) {
    currentValues.state = Jog;
    currentValues.stateColor = ILI9341_GREEN;
  } else if (strcmp(stateNew.c_str(), "Alarm") == 0) {
    currentValues.state = Alarm;
    currentValues.stateColor = ILI9341_RED;
  } else if (strcmp(stateNew.c_str(), "Door") == 0) {
    currentValues.state = Door;
    currentValues.stateColor = ILI9341_ORANGE;
  } else if (strcmp(stateNew.c_str(), "Home") == 0) {
    currentValues.state = Home;
    currentValues.stateColor = ILI9341_PURPLE;
  } else if (strcmp(stateNew.c_str(), "Hold") == 0) {
    currentValues.state = Hold;
    currentValues.stateColor = ILI9341_DARKGREY;
  } else if (strcmp(stateNew.c_str(), "Check") == 0) {
    currentValues.state = Check;
    currentValues.stateColor = 0xFFFF;
  } else if (strcmp(stateNew.c_str(), "Cycle") == 0) {
    currentValues.state = Cycle;
    currentValues.stateColor = 0xFFFF;
  } else if (strcmp(stateNew.c_str(), "Sleep") == 0) {
    currentValues.state = Sleep;
    currentValues.stateColor = 0xFFFF;
  }
  if (lastValues.state != currentValues.state) {
    currentValues.isStateSet = false;
  }
}

void setPositions(string MPos) {
  lastValues.x = currentValues.x;
  lastValues.y = currentValues.y;
  lastValues.z = currentValues.z;
  lastValues.a = currentValues.a;

  list<string> positionsList = splitString(MPos.c_str(), ",");
  int counter = 0;

  while (positionsList.size() != 0) {
    switch (counter) {
      case 0: {
        currentValues.x = (positionsList.front()).c_str();
        String mamad = String(currentValues.x.c_str());
        double temp = round(mamad.toDouble() * 100.0) / 100.0;
        char buffer[10];
        sprintf(buffer, "%.1lf", temp);
        currentValues.x = buffer;

        int xChange = currentValues.x.compare(lastValues.x);
        if (xChange != 0) {
          currentValues.isXSet = false;
          log_v("X_CURRENT-->  %s", currentValues.x.c_str());
        }
        break;
      }
      case 1: {
        currentValues.y = (positionsList.front()).c_str();
        String mamad = String(currentValues.y.c_str());
        double temp = round(mamad.toDouble() * 100.0) / 100.0;
        char buffer[10];
        sprintf(buffer, "%.1lf", temp);
        currentValues.y = buffer;

        int yChange = currentValues.y.compare(lastValues.y);
        if (yChange != 0) {
          currentValues.isYSet = false;
          log_v("Y_CURRENT-->  %s", currentValues.y.c_str());
        }
        break;
      }
      case 2: {
        currentValues.z = (positionsList.front()).c_str();
        String mamad = String(currentValues.z.c_str());
        double temp = round(mamad.toDouble() * 100.0) / 100.0;
        char buffer[10];
        sprintf(buffer, "%.1lf", temp);
        currentValues.z = buffer;

        int zChange = currentValues.z.compare(lastValues.z);
        if (zChange != 0) {
          currentValues.isZSet = false;
          log_v("Z_CURRENT-->  %s", currentValues.z.c_str());
        }
        break;
      }
      case 3: {
        currentValues.a = (positionsList.front()).c_str();
        String mamad = String(currentValues.a.c_str());
        double temp = round(mamad.toDouble() * 100.0) / 100.0;
        char buffer[10];
        sprintf(buffer, "%.1lf", temp);
        currentValues.a = buffer;

        int aChange = currentValues.a.compare(lastValues.a);
        if (aChange != 0) {
          currentValues.isASet = false;
          log_v("A_CURRENT-->  %s", currentValues.a.c_str());
        }
        break;
      }
      default:
        break;
    }
    positionsList.pop_front();
    counter++;
  }
}

void setFeedRateOV(string OV) {
  log_v("OV--> %s", OV.c_str());
  lastValues.feedRateOV = currentValues.feedRateOV;

  list<string> ovList = splitString(OV.c_str(), ",");
  int counter = 0;

  currentValues.feedRateOV = (ovList.front()).c_str();
  int ovChanged = currentValues.feedRateOV.compare(lastValues.feedRateOV);
  if (ovChanged != 0) {
    currentValues.isFeedRateOVSET = false;
    log_v("Current_FeedRateOV-->  %s", currentValues.feedRateOV.c_str());
  }
}

void setFeedRates(string feedRates) {
  lastValues.feedRate = currentValues.feedRate;
  lastValues.spindleRate = currentValues.spindleRate;

  list<string> feedRateList = splitString(feedRates.c_str(), ",");
  int counter = 0;

  currentValues.feedRate = (feedRateList.front()).c_str();
  int feedRate = currentValues.feedRate.compare(lastValues.feedRate);
  if (feedRate != 0) {
    currentValues.isFeedRateSet = false;
    log_v("Current_FEEDRATE-->  %s", currentValues.feedRate.c_str());
  }

  currentValues.spindleRate = (feedRateList.back()).c_str();
  int spindleRate = currentValues.spindleRate.compare(lastValues.spindleRate);
  if (spindleRate != 0) {
    currentValues.isSpindleRateSet = false;
    log_v("Current_SPINDLERATE-->  %s", currentValues.spindleRate.c_str());
  }
}

void setRunProgress(string progressVal) {
  lastValues.progress = currentValues.progress;

  list<string> progressList = splitString(progressVal.c_str(), ",");
  int counter = 0;

  currentValues.progress = (progressList.front()).c_str();
  int progress = currentValues.progress.compare(lastValues.progress);
  if (progress != 0) {
    currentValues.isProgressSet = false;
    log_v("Current_PROGRESS-->  %s", currentValues.progress.c_str());
  }
}

void parseRecieved(string data) {
  // example input
  //<Door:0|MPos:0.000,0.000,0.000,0.000|FS:0,0|Pn:P|WCO:0.000,0.000,0.000,0.000>
  log_v("RECIEVED: %s", data.c_str());
  if (page.currentPage == 2 && !SD.refresh) {
    if (isContain(data.c_str(), "FILE")) {
      // Example File --->   [FILE:/System Volume
      // Information/WPSettings.dat|SIZE:12]
      std::string firstLayer = splitString(data.c_str(), "|").front();
      std::list<string> allFiles = splitString(firstLayer.c_str(), "/");
      if (allFiles.size() == 2)  // import just root file
      {
        if (isContain(allFiles.back().c_str(), "TAP") ||
            isContain(allFiles.back().c_str(), "tap") ||
            isContain(allFiles.back().c_str(),
                      "Tap"))  // import just .TAP files
        {
          SD.items.push_back(allFiles.back().c_str());
        }
      }
    } else if (isContain(data.c_str(), "SD")) {
      SD.refresh = true;
    }
  } else {
    /* code */
    if (isContain(data.c_str(), "<"))  // response for (?)
    {
      list<string> parsedData =
          splitString(cutExtraChar(data.c_str()).c_str(), "|");

      setState(parsedData.front());
      parsedData.pop_front();

      for (string splited : parsedData) {
        if (isContain(splited, "MPos") || isContain(splited, "WPos")) {
          // Positions
          setPositions((splited.erase(0, 5)).c_str());
        } else if (isContain(splited, "FS")) {
          // feedRate and spindleRate
          setFeedRates((splited.erase(0, 3)).c_str());
        } else if (isContain(splited, "Pn")) {
          log_v("Pn: %s", splited);
        } else if (isContain(splited, "WCO")) {
          log_v("WCO: %s", splited);
        } else if (isContain(splited, "SD")) {
          // progress percent of file running
          setRunProgress((splited.erase(0, 3)).c_str());
        } else if (isContain(splited, "Ov")) {
          // overwrite percent of feedrate and spindlerate
          setFeedRateOV((splited.erase(0, 3)).c_str());
        }
      }
    }
  }
}

void recieveTask(void *p) {
  const byte numChars = 150;
  char receivedChars[numChars];
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  boolean newData = false;
  while (true) {
    while (Serial1.available() > 0 && newData == false) {
      rc = Serial1.read();

      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
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