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


class SensorService {

public:
static SensorService& getInstance();

void add_sensor(adc_channel_t channel);
void calibrate_sensor(adc_channel_t channel, int step_size, int range);

private:

static std::unique_ptr<SensorService> instance;
SensorService() = default;
SensorService(const SensorService&) = delete;
SensorService& operator=(const SensorService&) = delete;

std::map<adc_channel_t, std::unique_ptr<Sensor>> sensors; // Map to hold sensors by channel
};

} // namespace new