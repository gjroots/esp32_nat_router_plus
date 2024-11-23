/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdbool.h"

    extern bool IsLedEnable;
    void start_led_status_task(void);
    void item_selected_success(void);

#ifdef __cplusplus
}
#endif