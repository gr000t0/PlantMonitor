#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_vfs_dev.h"


#include "sensor_service.hpp"

static const char* TAG = "moisture_sensor";

void init_console(void) {
    esp_console_config_t console_config = ESP_CONSOLE_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_console_init(&console_config) );
}


void start_console_repl(void) {
        // 1) REPL-Konfig
    esp_console_repl_config_t repl_cfg = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    // 2) UART-Device-Konfig
    esp_console_dev_uart_config_t uart_cfg = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    // 3) Handle
    esp_console_repl_t *repl = nullptr;

    // 4) REPL anlegen (installiert Treiber + VFS automatisch)
    ESP_ERROR_CHECK( esp_console_new_repl_uart(&uart_cfg, &repl_cfg, &repl) );
    // 5) REPL starten
    ESP_ERROR_CHECK( esp_console_start_repl(repl) );
}


extern "C" void app_main() {
  
    init_console();

    sensor::SensorService& sensor_service = sensor::SensorService::getInstance();
    sensor_service.add_sensor(ADC_CHANNEL_3); 
    sensor_service.register_commands();
    start_console_repl();

}