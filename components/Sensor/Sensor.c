#include "sensor.h"
#include "freertos/FreeRTOS.h"   // FreeRTOS macros and definitions
#include "freertos/task.h"        


// Initialize PIR sensor
void pir_init() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << SENSOR_GPIO_NUM_6),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
};
}

// Check if PIR sensor detects motion
int pir_motion_detected() 
{
    int motion = gpio_get_level(SENSOR_GPIO_NUM_6);
   vTaskDelay(pdMS_TO_TICKS(10));  // Debounce delay to reduce false triggers
    return motion;  // Returns 1 if motion is detected
};
