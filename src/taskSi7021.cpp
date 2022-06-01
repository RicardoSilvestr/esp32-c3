#include <SPI.h>
#include "./common.h"
#include "./params.h"
#include "Adafruit_Si7021.h"

void TaskSi7021(void* pvParameters) {
  Adafruit_Si7021 sensor = Adafruit_Si7021();
  vTaskDelay(100);
  (void)pvParameters;

  Wire.begin(
      3, 4);  // Define specific i2c pins for SDA/SCL (mandatory for ESP32-C3)

  while (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    vTaskDelay(300);
  }

  while (true) {
    setParameter(PARAM_INT_TEMPERATURE, 100 * sensor.readTemperature());
    setParameter(PARAM_INT_HUMIDITY, 100 * sensor.readHumidity());

    vTaskDelay(1000);
  }
}

void taskSi7021() {
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(TaskSi7021, "TaskSi7021",
                          2048,  // This stack size can be checked & adjusted by
                                 // reading the Stack Highwater
                          NULL,
                          2,  // Priority, with 3 (configMAX_PRIORITIES - 1)
                              // being the highest, and 0 being the lowest.
                          NULL, 1);
}