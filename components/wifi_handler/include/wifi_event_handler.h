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

#include <freertos/event_groups.h>
#include <esp_event.h>
#include <esp_err.h>
#define JOIN_TIMEOUT_MS (3000)

    EventGroupHandle_t wifi_event_group;
    const int WIFI_CONNECTED_BIT;
    void wifi_events_register_init(void);
    bool is_scanning_progress;

#ifdef __cplusplus
}
#endif
