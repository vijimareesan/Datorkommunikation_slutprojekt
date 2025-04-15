#ifndef BUTTON_H
#define BUTTON_H

#include "driver/gpio.h"
#include <stdbool.h> 
#include "buzzer.h"
#include <stdbool.h>


// Define the button pin
#define PIN 2
//extern bool ButtonPressedState = false;

typedef struct button{
    int pin;
    bool lastState;
    bool isPressed;
    bool waspressed;
    void (*onPressed)(int pin);
} Button_t;

void init(Button_t *btn, int pin);
void button_update(Button_t *btn);
bool button_isPressed(Button_t *btn);
void button_setOnPressed(Button_t *btn, void (*onPressed)(int pin));
void stop_alarm_callback(int pin);
#endif


