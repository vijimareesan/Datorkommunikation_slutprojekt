#include "button.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "buzzer.h"
#include <stdio.h>

#define DEBOUNCE_DELAY_MS 100  

#define PIN 2

static const char* TAG = "Button";
extern Button_t button;  
static bool alarm_on = false;


void init(Button_t *btn, int pin) {
    btn->pin = pin;
    btn->lastState = false;
    btn->isPressed = false;
    btn->onPressed = NULL;

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };
    gpio_config(&io_conf);
}

void button_update(Button_t *btn)
{
    static uint32_t lastUpdateTime = 0;
    uint32_t currentTime = xTaskGetTickCount() * portTICK_PERIOD_MS;

    if (currentTime - lastUpdateTime < DEBOUNCE_DELAY_MS) 
    {
        return;
    }

    lastUpdateTime = currentTime;

    bool currentState = (gpio_get_level(btn->pin) == 1);
   
    if (currentState != btn->lastState) 
    {
        btn->lastState = currentState;

        if (currentState) {
            btn->isPressed = true;
            
            if (btn->onPressed) 
            {
                ESP_LOGI(TAG, "Button Update called :onPressed");
                btn->onPressed(btn->pin);
            }
        } else {
            btn->isPressed = false;
        
            ESP_LOGI(TAG, "Button Released.");
        }
    }
}

bool button_isPressed(Button_t *btn)
{ 
    return btn->isPressed;
}

void button_setOnPressed(Button_t *btn, void (*onPressed)(int pin)) {
    bool testval = btn->onPressed ;

    printf("button_onPressed: %s : \n", testval ? "true" : "false");
     btn->onPressed = onPressed;
   
}

void stop_alarm_callback(int pin) {
    ESP_LOGI(TAG, "Stopping alarm due to button press on GPIO %d", pin);
    alarm_on = false;  
    buzzer_off();
}