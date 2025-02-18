/*
 * uss.c
 *
 *  Created on: Jan 22, 2025
 *      Author: joseph
 */

#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include "gpio.h"
#include "timer.h"

#define SPEED_OF_SOUND 343
#define US_PER_SECOND 1000000
#define CM_PER_METER 100
#define NUM_READINGS 10
#define MAX_DIST_CLOCK_CYCLES 186589    // 4 meters

void uss_init(void)
{
    // Initialize uss_timer
    timer_uss_timer_init();

    // Initialize trigger pin
    gpio_pin_init(GPIOP, 0, OUTPUT, NO_PULL);
}

uint16_t uss_get_distance(void)
{
    uint32_t echo_start_time, echo_end_time, echo_time;
    uint32_t min_echo_time = MAX_DIST_CLOCK_CYCLES;
    uint16_t distance_cm;

    uint8_t i;
    for (i = 0; i < NUM_READINGS; i++) {
        TIMER1->ICR |= TIMER_ICR_CAECINT;                       // Clear capture interrupt flag
        // Send trigger pulse for > 10 us
        GPIOP->DATA ^= BIT0;
        timer_delay_us(10);
        GPIOP->DATA ^= BIT0;

        // Calculate echo time
        while ((TIMER1->RIS & TIMER_RIS_CAERIS) == 0) {;}       // Wait until rising edge is detected
        TIMER1->ICR |= TIMER_ICR_CAECINT;                       // Clear capture interrupt flag
        echo_start_time = (TIMER1->TAPS << 16) + TIMER1->TAR;   // Get start time

        while ((TIMER1->RIS & TIMER_RIS_CAERIS) == 0) {;}       // Wait until falling edge is detected
        TIMER1->ICR |= TIMER_ICR_CAECINT;                       // Clear capture interrupt flag
        echo_end_time = (TIMER1->TAPS << 16) + TIMER1->TAR;     // Get end time

        // Calculate time
        if (TIMER1->RIS & TIMER_RIS_TATORIS) {                  // If time-out event reached, calculate wrap-around
            echo_time = (PRESCALE << 16) + INTERVAL_LOAD - echo_start_time + echo_end_time;
        }
        else {
            echo_time = echo_end_time - echo_start_time;
        }
        TIMER1->ICR |= TIMER_ICR_TATOCINT;                      // Clear time-out interrupt flag

        if (echo_time < min_echo_time)
            min_echo_time = echo_time;

        timer_delay_us(10000);
    }

    // Convert clock cycles to centimeters
    distance_cm = ((uint64_t)SPEED_OF_SOUND * min_echo_time * CM_PER_METER) / SystemCoreClock;
    // Halve the distance
    distance_cm /= 2;

    return distance_cm;
}

uint16_t uss_set_max_distance(void)
{
    uint16_t cur_distance, avg_distance = 0;

    uint8_t i;
    for (i = 0; i < NUM_READINGS; i++) {
        cur_distance = uss_get_distance();
        avg_distance += cur_distance;
        timer_delay_us(10000);
    }
    avg_distance /= NUM_READINGS;

    return avg_distance;
}
