/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */
#ifndef HARDWARE_HANDLER_H
#define HARDWARE_HANDLER_H

#endif

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdbool.h>

    void hardware_init();
    bool hardware_user_button_pressed(void);
    void hardware_turn_on_led(void);
    void hardware_turn_off_led(void);
    void hardware_toggle_led(void);

#ifdef __cplusplus
}
#endif
