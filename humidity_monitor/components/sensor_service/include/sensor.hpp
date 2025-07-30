
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor_service.hpp"

namespace sensor {
class Sensor {

    public: 

    adc_channel_t channel; 

    sensor::dryness_t current_dryness;

    bool calibrated;

    TaskHandle_t monitor_task_handle;

    Sensor(adc_channel_t channel);

    void start();

    void init_sensor();
    int read_sensor();

    void set_dry_threshold();
    void set_wet_threshold();
    void set_step_size(int step);
    void set_range(int range);

    dryness_t get_current_dryness();

    void monitor_thread(void *arg);



    private:
    adc_oneshot_unit_handle_t adc_handle; 
    int dry_threshold;
    int wet_threshold;
    int step_size;
    int range;



};
}// namespace sensor