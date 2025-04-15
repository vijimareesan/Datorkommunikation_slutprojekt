
#include "hal/adc_types.h"
#include "potentiometer.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define POT_PIN 3
adc_oneshot_unit_handle_t adc1_handle;

// Initialize ADC for the potentiometer
/*void potentiometer_init() {
    adc_oneshot_unit_init_cfg_t adc_init_config = {
        .unit_id = ADC_UNIT_1
    };
    adc_oneshot_new_unit(&adc_init_config, &adc1_handle);

    adc_oneshot_chan_cfg_t adc_config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12
    };
    adc_oneshot_config_channel(adc1_handle, POT_PIN, &adc_config);
     ESP_LOGI("Potentiometer", "Potentiometer initialized on GPIO %d", POT_PIN);
}*/


void potentiometer_init() {
    gpio_config_t pot_config = {
        .pin_bit_mask = (1ULL << POT_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&pot_config);
    ESP_LOGI("Potentiometer", "Potentiometer initialized on GPIO %d", POT_PIN);
    
    // ADC setup
    adc_oneshot_unit_init_cfg_t adc_init_config = {
        .unit_id = ADC_UNIT_1
    };
    adc_oneshot_new_unit(&adc_init_config, &adc1_handle);

    adc_oneshot_chan_cfg_t adc_config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_11
    };
    adc_oneshot_config_channel(adc1_handle, POT_PIN, &adc_config);
}

uint32_t potentiometer_read(int pin) 
{
    int raw_value = 0;  // Declare and initialize raw_value
    adc_oneshot_read(adc1_handle, pin, &raw_value); 
    ESP_LOGI("Potentiometer", "Raw ADC Value: %d", raw_value);

    // Now, map this value to an alarm duration (1-10 seconds)
    uint32_t buzzer_duration = map_potentiometer_to_duration(raw_value);

    return buzzer_duration;
}

uint32_t map_potentiometer_to_duration(uint32_t value) 
{
    // Map ADC value (0-4095) to 1000ms (1s) to 10000ms (10s)
    return (value * 9000) / 4095 + 1000;  // Maps to 1-10 seconds range
}