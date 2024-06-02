#include "serialPort.h"

void serialSetup()
{
  xTaskCreate(recieveTask, "recieveTask", 2 * 1024, NULL, 400, NULL);
  xTaskCreate(sendTask, "sendTask", 2 * 1024, NULL, 300, NULL);
}