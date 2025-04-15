idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES driver Potentiometer deep_sleep Buzzer Button Sensor)       