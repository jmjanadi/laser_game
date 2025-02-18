/*
 * lcd.h
 *
 *  Created on: Jan 22, 2025
 *      Author: joseph
 */
#include <ti/devices/msp432e4/driverlib/driverlib.h>

#ifndef LCD_H_
#define LCD_H_

/**
 * @brief Initializes the LCD in 4-bit mode
 *
 * For the NHD-0216BZ-RN-YBW
 */
void lcd_init(void);

/**
 * @brief Sets the LCD cursor position to the desired row and column
 *
 * The next character to be written to the LCD will be displayed at the specified position.
 *
 * @param row First row (0) or second row (1)
 * @param col First column (0) through 16th column (15)
 */
void lcd_set_position(uint8_t row, uint8_t col);

/**
 * @brief Displays a character on the LCD
 *
 * The character is displayed at the position of the cursor
 *
 * @param c The code of the character to be displayed
 */
void lcd_print_char(char c);

/**
 * @brief Displays a string on the LCD
 *
 * The string is displayed starting at the position of the cursor
 *
 * @param str Pointer to null-terminate string to be displayed
 */
void lcd_print_string(const char *str);

/**
 * @brief Displays a 16-bit number on the LCD
 *
 * @param num An unsigned 16-bit number to be printed
 */
void lcd_print_number(uint16_t num);

#endif /* LCD_H_ */
