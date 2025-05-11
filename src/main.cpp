#include "main.h"

void setup()
{
  Serial.begin(115200);
  Serial1.setPins(RX_PIN, TX_PIN);
  Serial1.setRxBufferSize(1024);
  Serial1.setTimeout(5000); 
  Serial1.begin(115200);

  serialSetup();
  displaySetup();
  keypadSetup();
  initBuzzer();
}

void loop()
{
  vTaskDelay(1);
}