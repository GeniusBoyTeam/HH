#include "main.h"

TaskFunction_t serialPortTask;

void setup()
{
  Serial.begin(115200);
  Serial1.setPins(18,17);
  Serial1.begin(115200);
  // turn "ON" lcd backlight
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  
  serialSetup();
  displaySetup();
}

void loop()
{
  vTaskDelay(1);
}