#include "sensor.hpp"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"



static const char* TAG = "sensor";

namespace sensor {




Sensor::Sensor(adc_channel_t channel) {
    this->channel = channel;
    this->calibrated = false;
    this->current_dryness = DRYNESS_UNKNOWN;
    this->dry_threshold = 0;
    this->wet_threshold = 0;
    this->step_size = 0;
    this->range = 0;

    this->init_sensor();
}

void Sensor::init_sensor() {

    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = ADC_UNIT_1,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE
    };

    esp_err_t ret = adc_oneshot_new_unit(&unit_config, &this->adc_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize ADC unit: %s", esp_err_to_name(ret));
        return;
    }

    adc_oneshot_chan_cfg_t channel_config = {
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };

    ret = adc_oneshot_config_channel(this->adc_handle, this->channel, &channel_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure ADC channel: %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Sensor initialized on channel %d", this->channel);
    }
}

int Sensor::read_sensor() {
    int adc_raw = 0;
    esp_err_t ret = adc_oneshot_read(this->adc_handle, this->channel, &adc_raw);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read sensor: %s", esp_err_to_name(ret));
        return -1; // Error reading sensor
    }
    
    return adc_raw; 
}

void Sensor::set_dry_threshold() {
    int n = 10; // Number of measurements to average
    int measurements[n];
    for (int i = 0; i < n; i++) {
        measurements[i] = this->read_sensor();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay between readings
    }
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += measurements[i];
    }
    this->dry_threshold = sum / n; // Average value
    ESP_LOGI(TAG, "Dry threshold set to %d", this->dry_threshold);
}

void Sensor::set_wet_threshold() {
    int n = 10; // Number of measurements to average
    int measurements[n];
    for (int i = 0; i < n; i++) {
        measurements[i] = this->read_sensor();
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay between readings
    }
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += measurements[i];
    }
    this->wet_threshold = sum / n; // Average value
    ESP_LOGI(TAG, "Wet threshold set to %d", this->wet_threshold);

}

void Sensor::set_step_size(int step) {
    this->step_size = step;
    ESP_LOGI(TAG, "Step size set to %d", this->step_size);
}

void Sensor::set_range(int range) {
    this->range = range;
    ESP_LOGI(TAG, "Range set to %d", this->range);
}

void Sensor::monitor_thread(void *arg) {
    while (true) {
        int raw_value = this->read_sensor();
        if (raw_value > (this->dry_threshold - this->range) ) {
            this->current_dryness = DRYNESS_DRY;
            if (raw_value > (this->dry_threshold + this->range + this->step_size)) {
                this->current_dryness = DRYNESS_VERY_DRY;
            }
        } else if (raw_value < this->wet_threshold + this->range) {
            this->current_dryness = DRYNESS_WET;
            if (raw_value < this->wet_threshold - (this->range + this->step_size)) {
                this->current_dryness = DRYNESS_VERY_WET;
            }
        } else {
            this->current_dryness = DRYNESS_UNKNOWN;
        }
        ESP_LOGI(TAG, "Current dryness: %d", this->current_dryness);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for next reading
    }

}

dryness_t Sensor::get_current_dryness() {
    return this->current_dryness;
}

void Sensor::start() {
    if (this->monitor_task_handle == nullptr) {
        xTaskCreate(&Sensor::taskEntry, "sensor_monitor", 2048, this, 5, &this->monitor_task_handle);
        ESP_LOGI(TAG, "Sensor monitoring started");
    } else {
        ESP_LOGW(TAG, "Sensor monitoring already running");
    }
}


} // namespace sensor