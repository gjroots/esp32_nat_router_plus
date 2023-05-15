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

#include <esp_err.h>
    esp_err_t get_portmap_tab();
    esp_err_t apply_portmap_tab();
    esp_err_t delete_portmap_tab();

#ifdef __cplusplus
}
#endif
