/**
 * @author Jaya Satish
 * 
 *@copyright Copyright (c) 2023
 *Licensed under MIT 
 *
 */
#include <driver/ledc.h>
#include <driver/gpio.h>

#include "hardware_handler.h"
#include "utils.h"
#include "led_handler.h"
#include "button_handler.h"
#include "router_globals.h"

#define PIN_BUTTON (0)
#define PIN_LED (2)

static bool led_on = false;


//-----------------------------------------------------------------------------
bool hardware_user_button_pressed(void)
{
    return !gpio_get_level(PIN_BUTTON);
}

//-----------------------------------------------------------------------------
void hardware_turn_on_led(void)
{
    gpio_set_level(PIN_LED, 1);
    led_on = true;
}

//-----------------------------------------------------------------------------
void hardware_turn_off_led(void)
{
    gpio_set_level(PIN_LED, 0);
    led_on = false;
}

//-----------------------------------------------------------------------------
void hardware_toggle_led(void)
{
    led_on = !led_on;
    gpio_set_level(PIN_LED, led_on);
}

//-----------------------------------------------------------------------------
void hardware_init()
{
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = (1ULL << PIN_BUTTON);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);

    io_conf.pin_bit_mask = (1ULL << PIN_LED);
    io_conf.mode = GPIO_MODE_OUTPUT;
    gpio_set_level(PIN_LED, led_on);
    gpio_config(&io_conf);

    start_button_task();
    start_led_status_task();
}
