#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "sensor_service.hpp"

static const char* TAG = "moisture_sensor";

extern "C" void app_main() {
  
sensor::SensorService service;
service.init_sensor(ADC_CHANNEL_3);
 int raw = 0;
    while (true) {
        raw = service.read_sensor(ADC_CHANNEL_3);
        ESP_LOGI(TAG, "Raw: %d", raw);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}