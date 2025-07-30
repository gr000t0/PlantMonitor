idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES 
                        log
                        driver
                        esp_adc
                        freertos
                        sensor_service)
