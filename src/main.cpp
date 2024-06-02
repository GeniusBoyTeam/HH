#include "main.h"

void setup()
{
  Serial.begin(115200);
  Serial1.setPins(RX_PIN, TX_PIN);
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