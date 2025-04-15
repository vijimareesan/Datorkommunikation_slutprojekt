
#ifndef SENSOR_H
#define SENSOR_H

#include "driver/gpio.h"

// Define the PIR sensor pin
#define SENSOR_GPIO_NUM_6 6

// Function declarations
void pir_init();
int pir_motion_detected();

#endif  