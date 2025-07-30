#pragma once
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"
#include <map>

namespace sensor {

class SensorService {
public:

std::map<adc_channel_t, adc_oneshot_unit_handle_t> sensor_handles; 

void init_sensor(adc_channel_t channel);

int read_sensor(adc_channel_t channel);

private:

  
};

} // namespace new