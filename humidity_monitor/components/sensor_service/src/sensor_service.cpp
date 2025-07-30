#include "sensor_service.hpp"
#include "esp_log.h"


static const char* TAG = "sensor_service";

namespace sensor {

std::unique_ptr<SensorService> SensorService::instance = nullptr;

SensorService& SensorService::getInstance() {
    if (!instance) {
        // Erzeugt die einzige Instanz beim ersten Zugriff
        instance = std::make_unique<SensorService>();
    }
    return *instance;
}

void SensorService::add_sensor(adc_channel_t channel) {
    if (sensors.find(channel) == sensors.end()) {
        sensors[channel] = std::make_unique<Sensor>(channel);
        ESP_LOGI(TAG, "Sensor added on channel %d", channel);
    } else {
        ESP_LOGW(TAG, "Sensor already exists on channel %d", channel);
    }
}

} // namespace sensor