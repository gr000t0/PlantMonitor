#include "sensor_service.hpp"
#include "esp_log.h"
#include "esp_err.h"
#include <cstring>


static const char* TAG = "sensor_service";

namespace sensor {

std::unique_ptr<SensorService> SensorService::instance = nullptr;

SensorService& SensorService::getInstance() {
    if (!instance) {
        // Erzeugt die einzige Instanz beim ersten Zugriff
        instance.reset(new SensorService());
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


void SensorService::start_monitoring() {
    for (auto& pair : sensors) {
        pair.second->start();
    }
    ESP_LOGI(TAG, "Started monitoring all sensors");
}


static int start_monitoring_cmd(int argc, char** argv) {
    SensorService::getInstance().start_monitoring();
    return ESP_OK;
}

// console commands for sensor service
static int set_step_size_cmd(int argc, char** argv) {
    if (argc != 3) {
        ESP_LOGE(TAG, "Usage: set_step_size <channel> <step_size>");
        return ESP_ERR_INVALID_ARG;
    }
    
    adc_channel_t channel = static_cast<adc_channel_t>(atoi(argv[1]));
    int step_size = atoi(argv[2]);
    
    SensorService::getInstance().get_sensor(channel).set_step_size(step_size);
    return ESP_OK;
}

static int set_range_cmd(int argc, char** argv) {
    if (argc != 3) {
        ESP_LOGE(TAG, "Usage: set_range <channel> <range>");
        return ESP_ERR_INVALID_ARG;
    }
    
    adc_channel_t channel = static_cast<adc_channel_t>(atoi(argv[1]));
    int range = atoi(argv[2]);
    
    SensorService::getInstance().get_sensor(channel).set_range(range);
    return ESP_OK;
}

static int set_dry_threshold_cmd(int argc, char** argv) {
    if (argc != 2) {
        ESP_LOGE(TAG, "Usage: set_dry_threshold <channel> <threshold>");
        return ESP_ERR_INVALID_ARG;
    }
    
    adc_channel_t channel = static_cast<adc_channel_t>(atoi(argv[1]));
    
    SensorService::getInstance().get_sensor(channel).set_dry_threshold();
    return ESP_OK;
}

static int set_wet_threshold_cmd(int argc, char** argv) {
    if (argc != 2) {
        ESP_LOGE(TAG, "Usage: set_wet_threshold <channel> <threshold>");
        return ESP_ERR_INVALID_ARG;
    }
    
    adc_channel_t channel = static_cast<adc_channel_t>(atoi(argv[1]));
    
    SensorService::getInstance().get_sensor(channel).set_wet_threshold();
    return ESP_OK;
}

void SensorService::register_commands() {
    esp_console_cmd_t step_cmd;
    memset(&step_cmd, 0, sizeof(esp_console_cmd_t));
    step_cmd.command = "set_step_size";
    step_cmd.help = "Set the step size for a sensor";
    step_cmd.hint = "<channel> <step_size>";
    step_cmd.func = &set_step_size_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&step_cmd));
    
    esp_console_cmd_t range_cmd;
    memset(&range_cmd, 0, sizeof(esp_console_cmd_t));
    range_cmd.command = "set_range";
    range_cmd.help = "Set the range for a sensor";
    range_cmd.hint = "<channel> <range>";
    range_cmd.func = &set_range_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&range_cmd));


     esp_console_cmd_t dry_cmd;
    memset(&dry_cmd, 0, sizeof(esp_console_cmd_t));
    dry_cmd.command = "set_dry_threshold";
    dry_cmd.help = "Set the dry threshold for a sensor";
    dry_cmd.hint = "<channel>";
    dry_cmd.func = &set_dry_threshold_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&dry_cmd));
    

    esp_console_cmd_t wet_cmd;
    memset(&wet_cmd, 0, sizeof(esp_console_cmd_t));
    wet_cmd.command = "set_wet_threshold";
    wet_cmd.help = "Set the wet threshold for a sensor";
    wet_cmd.hint = "<channel>";
    wet_cmd.func = &set_wet_threshold_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&wet_cmd));


    esp_console_cmd_t start_cmd;
    memset(&start_cmd, 0, sizeof(esp_console_cmd_t));
    start_cmd.command = "start_monitoring";
    start_cmd.help = "Start monitoring all sensors";
    start_cmd.hint = "";
    start_cmd.func = &start_monitoring_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&start_cmd));

}

} // namespace sensor