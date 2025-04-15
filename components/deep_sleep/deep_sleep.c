#include <stdio.h>
#include "deep_sleep.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "sdkconfig.h"
#include "esp_log.h" 
#include "driver/rtc_io.h"


#define CONFIG_EXAMPLE_EXT1_WAKEUP_MOD ESP_EXT1_WAKEUP_ANY_HIGH// Wake-up mHode for EXT1 (any high)

#define BUTTON_GPIO_NUM_2 2 // Pin 1 for EXT1 wake-up
#define SENSOR_GPIO_NUM_6 6// Pin 2 for EXT1 wake-up
#define CONFIG_EXAMPLE_EXT1_WAKEUP_PIN_1 BUTTON_GPIO_NUM_2  // Pin 1 for EXT1 wake-up
#define CONFIG_EXAMPLE_EXT1_WAKEUP_PIN_2 SENSOR_GPIO_NUM_6  // Pin 2 for EXT1 wake-up

// CONFIG_EXAMPLE_EXT1_WAKEUP
void example_deep_sleep_register_ext1_wakeup(void)
{
    const int ext_wakeup_pin_1 = BUTTON_GPIO_NUM_2;
    const int ext_wakeup_pin_2 = SENSOR_GPIO_NUM_6;
    const uint64_t ext_wakeup_pin_1_mask = 1ULL << ext_wakeup_pin_1;
    const uint64_t ext_wakeup_pin_2_mask = 1ULL << ext_wakeup_pin_2;
    printf("Enabling EXT1 wakeup on pins GPIO%d, GPIO%d\n", ext_wakeup_pin_1, ext_wakeup_pin_2);

      printf("EXT1 configured for GPIO2 and GPIO6 wake-up\n");


    ESP_ERROR_CHECK(esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_1_mask | ext_wakeup_pin_2_mask, ESP_EXT1_WAKEUP_ANY_HIGH));

}
 

