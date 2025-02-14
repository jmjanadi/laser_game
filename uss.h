/*
 * uss.h
 *
 *  Created on: Jan 27, 2025
 *      Author: joseph
 */

#ifndef USS_H_
#define USS_H_

/**
 * @brief Initializes US sensor
 *
 * Calls timer_uss_timer_init() to initialize the USS timer
 * and initializes GPIO pin P0 to connect to the USS trigger pin.
 */
void uss_init(void);

/**
 * @brief Calculates the distance to an object
 *
 * Sends trigger pulse, measures echo pulse width, and
 * calculates distance in centimeters.
 *
 * @return The 16-bit distance in centimeters
 */
uint16_t uss_get_distance(void);

/**
 * @brief Averages the first 10 distance readings
 *
 * @return The average 16-bit distance in centimeters
 */
uint16_t uss_set_max_distance(void);

#endif /* USS_H_ */
