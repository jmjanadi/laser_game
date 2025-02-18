/*
 * timer.c
 *
 *  Created on: Jan 28, 2025
 *      Author: joseph
 */

#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include "timer.h"

void timer_delay_timer_init(void)
{
    // Enable TIMER0 clock
    SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R0;
    while ((SYSCTL->PRTIMER & SYSCTL_PRTIMER_R0) == 0) {;}  // Wait until TIMER0 clock is enabled
    // Configure TIMER0 to concatenated (32-bit) mode
    TIMER0->CFG = TIMER_CFG_32_BIT_TIMER;

    // Disable TIMER0
    TIMER0->CTL &= ~TIMER_CTL_TAEN;
    // Configure TIMER0 for One-Shot mode
    TIMER0->TAMR |= TIMER_TAMR_TAMR_1_SHOT;
    // Configure TIMER0 for count-down mode
    TIMER0->TAMR &= ~TIMER_TAMR_TACDIR;
}

__attribute__((always_inline)) inline void timer_delay_us(uint64_t time_us)
{
    // Convert microseconds to number of clock cycles
    uint32_t load_value = time_us * SystemCoreClock / 1000000;

    // Start delay timer
    TIMER0->CTL &= ~TIMER_CTL_TAEN; // Disable timer
    TIMER0->TAILR = load_value - 1; // Set load value
    TIMER0->CTL |= TIMER_CTL_TAEN;  // Enable timer

    // Wait for delay timer to reach time-out event
    while ((TIMER0->RIS & TIMER_RIS_TATORIS) == 0) {;}  // Poll raw interrupt status bit until set
    TIMER0->ICR |= TIMER_ICR_TATOCINT;                  // Clear raw interrupt status bit
}

void timer_uss_timer_init(void)
{
    // Configure GPIO D2 for CCP (T1CCP0)
    SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R3; // Enable GPIO port D clock
    GPIOD->PDR |= BIT2;                     // Set internal pull-down
    GPIOD->ODR &= ~BIT2;                    // Clear open drain bit
    GPIOD->AFSEL |= BIT2;                   // Enable AFSEL pin 2
    GPIOD->PCTL |= (3 << 8);                // Set PMC2 to 3 (T1CCP0)
    GPIOD->DEN |= BIT2;                     // Set DEN pin 2

    // Enable TIMER1 clock
    SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R1;
    while ((SYSCTL->PRTIMER & SYSCTL_PRTIMER_R1) == 0) {;}

    // Configure TIMER1A
    TIMER1->CTL &= ~TIMER_CTL_TAEN;                             // Disable timer
    TIMER1->CFG = TIMER_CFG_16_BIT;                             // Individual/split (16-bit) mode
    TIMER1->TAMR |= (TIMER_TAMR_TAMR_CAP | TIMER_TAMR_TACMR | TIMER_TAMR_TACDIR);   // Edge-time capture mode, count-up
    TIMER1->CTL |= TIMER_CTL_TAEVENT_BOTH;                      // Event for both edges
    TIMER1->TAPR = PRESCALE;                                    // Prescale value
    TIMER1->TAILR = INTERVAL_LOAD;                              // Interval load value
    //TIMER1->IMR |= TIMER_IMR_CAEIM;                             // Enable capture mode event interrupts
    TIMER1->CTL |= TIMER_CTL_TAEN;                              // Enable timer
}

/**
 * @brief Captures timer value at rising/falling edge
 *
 * CURRENTLY NOT IN USE
 */
/*
void TIMER1A_IRQHandler(void) {
    capture_time = TIMER1->TAR;

    if (GPIOD->DATA & BIT2) { // If rising edge, set start time
        echo_start_time = capture_time;
    }
    else { // If falling edge, calculate echo time
        if (TIMER1->RIS & TIMER_RIS_TATORIS) { // If time-out event reached
            echo_time = PRESCALE + INTERVAL_LOAD - echo_start_time + capture_time;
        }
        else {
            echo_time = capture_time - echo_start_time;
        }
    }

    // Clear capture and time-out interrupt flags
    TIMER1->ICR |= (TIMER_ICR_CAECINT | TIMER_ICR_TATOCINT);
}
*/
