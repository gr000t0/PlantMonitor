#pragma once
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sensor.hpp"
#include <map>
#include <memory>

namespace sensor {

enum dryness_t {
    DRYNESS_UNKNOWN = 0,
    DRYNESS_DRY,
    DRYNESS_WET,
    DRYNESS_VERY_WET,
    DRYNESS_VERY_DRY
} ;

class SensorService {

public:
static SensorService& getInstance();

void add_sensor(adc_channel_t channel);

private:

static std::unique_ptr<SensorService> instance;
SensorService() = default;
SensorService(const SensorService&) = delete;
SensorService& operator=(const SensorService&) = delete;

std::map<adc_channel_t, std::unique_ptr<Sensor>> sensors; // Map to hold sensors by channel
};

} // namespace new