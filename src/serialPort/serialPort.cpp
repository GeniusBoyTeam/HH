#include "serialPort.h"

void serialSetup()
{
  xTaskCreate(recieveTask, "recieveTask", 10 * 2048, NULL, 400, NULL);
  xTaskCreate(sendTask, "sendTask", 10 * 2048, NULL, 300, NULL);
}