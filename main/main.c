#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "buzzer.h"
#include "button.h"
#include "sensor.h"
#include "potentiometer.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "deep_sleep.h"
#include <time.h>
#include <sys/time.h>
#include "sdkconfig.h"
#include "soc/soc_caps.h"
#include "driver/rtc_io.h"
#include <sys/time.h>


static struct timeval sleep_enter_time;
RTC_DATA_ATTR static bool alarm_on = false;
static Button_t button; 



void IRAM_ATTR button_isr_handler(void* arg) {
    
    // Update button state when the interrupt occurs
    button_update(&button);

    // Check if the button is pressed
    if (button_isPressed(&button)) 
    {
        printf("Button was pressed, stopping alarm\n");
        stop_alarm_callback(button.pin);
        alarm_on = false;
    } else {
        printf("Button was not pressed\n");
    }
}


void deep_sleep_task(void *args)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    int sleep_time_ms = (now.tv_sec - sleep_enter_time.tv_sec) * 1000 + (now.tv_usec - sleep_enter_time.tv_usec) / 1000;

    // Wake-up Cause Handling
    switch (esp_sleep_get_wakeup_cause()) {
        case ESP_SLEEP_WAKEUP_EXT1:
            printf("Wake-up caused by EXT1 (button or sensor)\n");
            break;
        default:
            printf("Not a deep sleep reset\n");
            break;
    }

    // Wake-up Status Handling
    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1) 
    {
        uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();

        switch (wakeup_pin_mask)
         {
            case (1ULL << 2):
                printf("Wake-up triggered by button on GPIO %d\n", 2);
                vTaskDelay(50 / portTICK_PERIOD_MS); 

                // Step 2: Update the button state

                button_update(&button);

                // Step 3: Check if the button is pressed
                if (button_isPressed(&button)) {
                    printf("Button was pressed at wake-up, stopping alarm\n");
                    stop_alarm_callback(button.pin);                                 
                    alarm_on = false;
                } else {
                    printf("Button was not pressed at wake-up, keeping alarm on\n");                       
                    alarm_on = true;                    
                }
               
                break;

            case (1ULL << 6):
                printf("Wake-up triggered by sensor on GPIO %d\n", 6);
                alarm_on = true;  // Always turn alarm ON if motion is detected
                break;

            default:
                printf("Wake-up triggered by unknown source\n");
                break;
        }

        if (alarm_on) {
            uint32_t buzzer_duration = potentiometer_read(POT_PIN);  // Read potentiometer value to determine duration
            if (buzzer_duration < 200) buzzer_duration = 200;  // Ensure the minimum duration is 200ms
            trigger_alarm(buzzer_duration);  // Trigger the alarm with the calculated duration            
        }
        else {
            printf("Alarm has been stopped by button press, skipping alarm trigger\n");
        }

        printf("Time spent in deep sleep: %dms\n", sleep_time_ms);
    }

    // Delay before checking again (to ensure the task doesn't return)
    vTaskDelay(100 / portTICK_PERIOD_MS);  // Keeps the task alive

   
  
    // Enter deep sleep again
    printf("Entering deep sleep\n");
    gettimeofday(&sleep_enter_time, NULL);

    // Continue the deep sleep cycle indefinitely, the task never returns
    esp_deep_sleep_start();
}


void app_main(void)
{
    // Initialize components

    init(&button, 2); 
    button_setOnPressed(&button, stop_alarm_callback);
    
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(2, button_isr_handler, NULL); 

    pir_init();
    buzzer_init();
    potentiometer_init();
    // Enable EXT1 wakeup for the button and sensor
    example_deep_sleep_register_ext1_wakeup();

   
    // Create the deep sleep task
    xTaskCreate(deep_sleep_task, "deep_sleep_task", 4096, NULL, 5, NULL);
    
}

