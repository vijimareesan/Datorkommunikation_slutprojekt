
#ifndef BUZZER_H
#define BUZZER_H

#include <stdbool.h>
#include <stdint.h>

#define BUZZER_PIN 4  // Define the GPIO pin for the buzzer

void buzzer_init();
void buzzer_on();
void buzzer_off();
void trigger_alarm(uint32_t duration);
bool alarm_toggle(bool current_state);


#endif