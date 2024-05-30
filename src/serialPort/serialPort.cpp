#include "serialPort.h"

void serialSetup()
{
  xTaskCreate(recieveTask, "recieveTask", 10 * 1024, NULL, 1, NULL);
  xTaskCreate(sendTask, "sendTask", 10 * 1024, NULL, 1, NULL);
}