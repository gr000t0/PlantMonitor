#include "sensor_service.hpp"
#include "esp_log.h"


static const char* TAG = "sensor_service";

namespace sensor {

void SensorService::init_sensor(adc_channel_t channel) {
    adc_oneshot_unit_handle_t adc_handle;
    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = ADC_UNIT_1,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE
    };
    adc_oneshot_new_unit(&unit_config, &adc_handle);

    adc_oneshot_chan_cfg_t channel_config = {
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    adc_oneshot_config_channel(adc_handle, channel, &channel_config);
    ESP_LOGI(TAG, "Sensor initialized on channel %d", channel);
    this->sensor_handles[channel] = adc_handle; // Store the handle for the channel
}

int SensorService::read_sensor(adc_channel_t channel) {
    adc_oneshot_unit_handle_t handle = this->sensor_handles[channel];

    if (handle == nullptr) {
        ESP_LOGE(TAG, "Sensor handle for channel %d not found", channel);
        return -1; // Handle not found
    }
    int adc_raw = 0;
    esp_err_t ret = adc_oneshot_read(handle, channel, &adc_raw);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read sensor: %s", esp_err_to_name(ret));
        return -1; // Error reading sensor
    }
    
  
    return adc_raw; 
    
}

} // namespace sensor