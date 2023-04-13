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

#include <esp_netif.h>

    esp_netif_t *wifiAP;
    esp_netif_t *wifiSTA;
    void wifi_init();

#ifdef __cplusplus
}
#endif
