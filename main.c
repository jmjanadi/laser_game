/*****************************************************************************
*
* @file main.c
*
* Main file for laser_game
*
******************************************************************************/

#include "gpio.h"
#include "timer.h"
#include "lcd.h"
#include "uss.h"

int main(void)
{
    // Initialization
    timer_delay_timer_init();
    lcd_init();
    uss_init();

    lcd_set_position(0, 0);
    lcd_print_string("Initialized     ");
    timer_delay_us(1000000);
    lcd_set_position(0, 0);
    lcd_print_string("                ");

    // Debug pin
    gpio_pin_init(GPIOD, 1, OUTPUT, NO_PULL);

    // Buzzer pin
    gpio_pin_init(GPIOM, 6, OUTPUT, NO_PULL);

    uint16_t dist_in_cm;
    uint16_t max_distance = uss_set_max_distance();

    while (1) {
        // Loop every second
        //timer_delay_us(100000);

        // Calculate distance
        dist_in_cm = uss_get_distance();

        if (dist_in_cm < 0.9 * max_distance) {
            GPIOM->DATA ^= BIT6;
            timer_delay_us(500000);
            GPIOM->DATA ^= BIT6;
        }

        // Write distance to LCD
        lcd_set_position(0, 0);
        lcd_print_string("Distance:       ");
        lcd_set_position(0, 10);
        lcd_print_number(dist_in_cm);
    }
}
