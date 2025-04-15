#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "esp_adc/adc_oneshot.h"


#define POT_PIN ADC_CHANNEL_3

void potentiometer_init();
uint32_t potentiometer_read(int pin);
uint32_t map_potentiometer_to_duration(uint32_t value);
#endif