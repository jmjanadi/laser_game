/*
 * lcd.c
 *
 *  Created on: Jan 17, 2025
 *      Author: joseph
 *
 *  Controls the LCD NHD-0216BZ-RN-YBW
 *
 */

#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include "gpio.h"
#include "timer.h"
#include "lcd.h"

// Define high and low bus signals
#define LOW 0
#define HIGH 0xF

// Define GPIO bit masks
#define DB_BIT_MASK_OFFSET 0x03C    // GPIOE pins 0:3
#define RS_BIT_MASK_OFFSET 0x004    // GPIOM pin 0
#define RW_BIT_MASK_OFFSET 0x008    // GPIOM pin 1
#define E_BIT_MASK_OFFSET  0x010    // GPIOM pin 2

// Define LCD bus registers
#define HWREG32(x)      (*(volatile uint32_t *)(x))
#define DATA_BUS        (HWREG32(GPIO_PORTE_BASE + DB_BIT_MASK_OFFSET))
#define REGISTER_SELECT (HWREG32(GPIO_PORTM_BASE + RS_BIT_MASK_OFFSET))
#define READ_WRITE      (HWREG32(GPIO_PORTM_BASE + RW_BIT_MASK_OFFSET))
#define ENABLE          (HWREG32(GPIO_PORTM_BASE + E_BIT_MASK_OFFSET))


/**
 * @brief Sets clock enable on the LCD
 *
 * Sets the enable line high, waits, then sets it low for the LCD to receive the data on the data bus.
 */
static void nybble(void)
{
    ENABLE = HIGH;
    timer_delay_us(1);            // delay >300 ns
    ENABLE = LOW;
}

/**
 * @brief Sends a command instruction to the LCD
 *
 * @param cmd The command instruction in binary or hexadecimal
 */
static void command(uint8_t cmd)
{
    DATA_BUS = (cmd >> 4);  // place higher 4 bits on data bus
    REGISTER_SELECT = LOW;  // instruction mode
    READ_WRITE = LOW;       // write mode
    nybble();               // send bits
    DATA_BUS = cmd;         // place lower 4 bits on data bus
    nybble();               // send bits
}

void lcd_init(void)
{
    // Initialize data GPIO pins E0-E3
    int i;
    for (i = 0; i < 4; i++) {
        gpio_pin_init(GPIOE, i, OUTPUT, NO_PULL);
    }
    // Initialize control GPIO pins M0-M2
    for (i = 0; i < 3; i++) {
        gpio_pin_init(GPIOM, i, OUTPUT, NO_PULL);
    }

    timer_delay_us(40000);    // delay >40 ms
    DATA_BUS = 0x03;    // place 0x30 on data bus
    timer_delay_us(5000);     // delay 5ms
    nybble();           // command 0x30 = wake up
    timer_delay_us(160);      // delay 160 us
    nybble();           // command 0x30 = wake up #2
    timer_delay_us(160);      // delay 160 us
    nybble();           // command 0x30 = wake up #3
    timer_delay_us(160);      // delay 160 us
    DATA_BUS = 0x02;    // place 0x20 on data bus
    nybble();           // function set: 4-bit interface
    command(0x28);      // function set: 4-bit/2-line
    command(0x10);      // set cursor
    command(0x0F);      // display on; blinking cursor
    command(0x06);      // entry mode set
}

void lcd_set_position(uint8_t row, uint8_t col)
{
    row *= 0x04;            // row 0 (0x00) or 1 (0x04)
    DATA_BUS = row + 0x08;  // set higher 4 bits of DDRAM address
    REGISTER_SELECT = LOW;  // instruction mode
    READ_WRITE = LOW;       // write mode
    nybble();               // send higher 4 bits
    DATA_BUS = col;         // set lower 4 bits of DDRAM address
    nybble();               // send lower 4 bits
    timer_delay_us(100);
}

void lcd_print_char(char c)
{
    DATA_BUS = (c >> 4);    // place higher 4 bits on data bus
    REGISTER_SELECT = HIGH; // data mode
    READ_WRITE = LOW;       // write mode
    nybble();               // send higher 4 bits
    DATA_BUS = c;           // place lower 4 bits on data bus
    nybble();               // send lower 4 bits
    timer_delay_us(100);
}

void lcd_print_string(const char *str)
{
    int i = 0;
    while (str[i]) {
        lcd_print_char(str[i]);
        i++;
    }
}

void lcd_print_number(uint16_t num)
{
    char digit_array[5];

    int i = 0;

    // Add digits to char array in reverse order
    do {
        digit_array[i] = num % 10 + '0';
        num /= 10;
        i++;
    } while (num > 0);

    // Iterate back over array to print in correct order
    while (i > 0) {
        i--;
        lcd_print_char(digit_array[i]);
    }
}
