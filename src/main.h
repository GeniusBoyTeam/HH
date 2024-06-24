#if !defined(JAM_MAIN)
#define JAM_MAIN
#include <Arduino.h>
#include <FreeRTOS.h>

#include "serialPort/serialPort.h"
#include "display/lcd.h"
#include "inputs/keypad.h"
#include "outputs/buzzer.h"

void setSensorTask(TaskFunction_t t);
void setTbClientTask(TaskFunction_t t);

#endif // JAM_MAIN