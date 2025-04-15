#include "buzzer.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>
#include <stdbool.h>
#include "esp_log.h"
#include <sys/time.h>
#include "button.h"


#define BUZZER_PIN 4

static const char* TAG = "Buzzer";
//static bool alarm_on = false;



// Initialize the buzzer
void buzzer_init(void)
{
    gpio_config_t buzzer_config = {
        .pin_bit_mask = (1ULL << BUZZER_PIN),  // Configure BUZZER_PIN
        .mode = GPIO_MODE_OUTPUT,              // Set as output
        .pull_down_en = GPIO_PULLDOWN_DISABLE, // No pull-down required
        .pull_up_en = GPIO_PULLUP_DISABLE,     // No pull-up required
        .intr_type = GPIO_INTR_DISABLE         // No interrupt needed
    };

    gpio_config(&buzzer_config);

    gpio_set_level(BUZZER_PIN, 0);  // Ensure buzzer is OFF initially
    ESP_LOGI(TAG, "Buzzer initialized on GPIO %d", BUZZER_PIN);
}

// Function to turn the buzzer ON
void buzzer_on()
{
    gpio_set_level(BUZZER_PIN, 1); // Set the buzzer pin to HIGH
    ESP_LOGI(TAG, "Buzzer ON");
};

// Function to turn the buzzer OFF
void buzzer_off()
{
    gpio_set_level(BUZZER_PIN, 0); // Set the buzzer pin to LOW
    ESP_LOGI(TAG, "Buzzer OFF");
};


bool alarm_toggle(bool current_state)
{
    bool new_state = !current_state;  // Toggle the alarm state
    if (new_state) {
        ESP_LOGI("Alarm", "Alarm turned ON manually");
    } else {
        buzzer_off();  // Stop the alarm if it was turned off
        ESP_LOGI("Alarm", "Alarm turned OFF manually");
    }
    return new_state;  // Return the new state of the alarm
}
// Function to activate the buzzer for a duration
void trigger_alarm(uint32_t  buzzer_duration)
{
    buzzer_on();  // Turn the buzzer On
    vTaskDelay(pdMS_TO_TICKS(buzzer_duration));
   buzzer_off();  // Turn the buzzer OFF
};
/*void trigger_alarm_interruptible(uint32_t buzzer_duration, Button_t *btn)
{
    buzzer_on();
    uint32_t start_time = xTaskGetTickCount();
    uint32_t timeout_ticks = pdMS_TO_TICKS(buzzer_duration);

    while ((xTaskGetTickCount() - start_time) < timeout_ticks) {
        button_update(button);

        if (button_isPressed(button)) {
            ESP_LOGI("Alarm", "Button pressed, stopping alarm early");
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));  // Non-blocking, checks regularly
    }

    buzzer_off();
}*/
