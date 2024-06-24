#include "buzzer.h"

extern refreshVal currentValues;

void beepBuzzer() {
  digitalWrite(42, HIGH);
  vTaskDelay(30);
  digitalWrite(42, LOW);
  vTaskDelay(30);
}

void buzzerTask(void *p) {
  pinMode(42, OUTPUT);  // buzzer pin
  while (true) {
    if (currentValues.alarm) {
      if (currentValues.state == Alarm || currentValues.state == Door) {
        digitalWrite(42, HIGH);
        vTaskDelay(75);
        digitalWrite(42, LOW);
        vTaskDelay(75);
      } else if (currentValues.state == Home) {
        digitalWrite(42, HIGH);
        vTaskDelay(50);
        digitalWrite(42, LOW);
        vTaskDelay(100);
        digitalWrite(42, HIGH);
        vTaskDelay(50);
        digitalWrite(42, LOW);
        vTaskDelay(500);
      }else if (currentValues.state == Hold) {
        digitalWrite(42, HIGH);
        vTaskDelay(100);
        digitalWrite(42, LOW);
        vTaskDelay(200);
        digitalWrite(42, HIGH);
        vTaskDelay(100);
        digitalWrite(42, LOW);
        vTaskDelay(2000);
      }
    }
    vTaskDelay(2);
  }
}

void initBuzzer() {
  xTaskCreate(buzzerTask, "buzzerTask", 2 * 2048, NULL, 300, NULL);
}