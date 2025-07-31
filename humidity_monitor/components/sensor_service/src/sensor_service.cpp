#include "sensor_service.hpp"
#include "esp_log.h"


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
    esp_console_cmd_t cmd;
    
    cmd.command = "set_step_size";
    cmd.help = "Set the step size for a sensor";
    cmd.hint = "<channel> <step_size>";
    cmd.func = &set_step_size_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
    
    cmd.command = "set_range";
    cmd.help = "Set the range for a sensor";
    cmd.hint = "<channel> <range>";
    cmd.func = &set_range_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
    
    cmd.command = "set_dry_threshold";
    cmd.help = "Set the dry threshold for a sensor";
    cmd.hint = "<channel>";
    cmd.func = &set_dry_threshold_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
    
    cmd.command = "set_wet_threshold";
    cmd.help = "Set the wet threshold for a sensor";
    cmd.hint = "<channel>";
    cmd.func = &set_wet_threshold_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));


    cmd.command = "start_monitoring";
    cmd.help = "Start monitoring all sensors";
    cmd.hint = "";
    cmd.func = &start_monitoring_cmd;
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
    
}

} // namespace sensor