/*
 * gpio.h
 *
 *  Created on: Jan 22, 2025
 *      Author: joseph
 */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

#ifndef GPIO_H_
#define GPIO_H_

enum IO {
    INPUT,
    OUTPUT
};

enum Pull_up_down {
    NO_PULL = -1,
    PULLUP,
    PULLDOWN
};

void gpio_pin_init(GPIO_Type *port, uint8_t pin, enum IO io, enum Pull_up_down pull_up_down);

#endif /* GPIO_H_ */
