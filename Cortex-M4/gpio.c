/*
 * gpio.c
 *
 *  Created on: Jan 22, 2025
 *      Author: joseph
 */

#include <ti/devices/msp432e4/driverlib/driverlib.h>

#include "gpio.h"

void gpio_pin_init(GPIO_Type *port, uint8_t pin, enum IO io, enum Pull_up_down pull_up_down)
{
    uintptr_t port_base_address = (uintptr_t)port;
    uint32_t port_num = ((uint32_t)port_base_address - 0x40058000) / 0x1000;
    uint16_t port_bit = 1 << port_num;

    uint8_t pin_bit = 1 << pin;

    // 1. Enable the clock to port
    SYSCTL->RCGCGPIO |= port_bit;

    // 2. Program pin as input or output
    if (io == OUTPUT) {
        port->DIR |= pin_bit;
    }
    else {
        port->DIR &= ~pin_bit;
    }

    // 3. Program pin as GPIO (not alternate function)
    port->AFSEL &= ~pin_bit;

    // 4. Set the EDMn field in the GPIOPC register to 0x0 (no extended drive)
    port->PC &= ~pin_bit;

    // 5.-7. Clear drive control register bits
    port->DR4R &= ~pin_bit;
    port->DR8R &= ~pin_bit;
    port->DR12R &= ~pin_bit;

    // 8. Program to have pull-up, pull-down, or open-drain functionality
    if (pull_up_down == PULLUP) {
        port->PUR |= pin_bit;
    }
    else if (pull_up_down == PULLDOWN) {
        port->PDR |= pin_bit;
    }
    else {
        port->PUR &= ~pin_bit;
        port->PDR &= ~pin_bit;
    }

    // 9. Enable as digital I/O
    port->DEN |= pin_bit;

    // 10. Configure the type, event, and mask of the interrupts
}
