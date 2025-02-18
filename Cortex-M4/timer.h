/*
 * timer.h
 *
 *  Created on: Jan 28, 2025
 *      Author: joseph
 */

#ifndef TIMER_H_
#define TIMER_H_

#define PRESCALE 0xFF
#define INTERVAL_LOAD 0xFFFF

/**
 * @brief Initializes Timer0 to function as a delay timer
 *
 * Initializes Timer0 as a concatenated 32-bit timer
 */
void timer_delay_timer_init(void);

/**
 * @brief Creates a delay using Timer0
 *
 * Converts the time delay argument into the corresponding number of clock cycles,
 * then loads that value into Timer0's interval load register, enables the timer,
 * and then waits until the time-out event is reached before exiting.
 *
 * @param time_us The amount of time to delay in microseconds
 */
inline void timer_delay_us(uint64_t time_us);

/**
 * @brief Initializes TIMER1A for echo pulse of US sensor
 *
 * TIMER1A is configured for edge-time capture mode
 * When the echo pin toggles high, the start time is recorded
 * When the echo pin toggles low, the echo time is calculated
 */
void timer_uss_timer_init(void);

#endif /* TIMER_H_ */
