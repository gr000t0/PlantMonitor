idf_component_register(
  SRCS 
    "src/sensor_service.cpp"
    "src/sensor.cpp"
  INCLUDE_DIRS "include"
  REQUIRES  
    log
    driver
    esp_adc
    freertos
    console
)