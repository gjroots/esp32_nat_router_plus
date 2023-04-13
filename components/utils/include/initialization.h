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

#include <stdint.h>
#include "esp_err.h"
#define MAX_LOGIN_ATTEMPTS (5)
#define LOCK_OUT_TIME_MINUTES (1) // MINUTES
#define DEFAULT_AP_IP "192.168.4.1"
#define DEFAULT_DNS1 "1.1.1.1"
#define DEFAULT_ADMIN_USERNAME "admin"
#define DEFAULT_ADMIN_PASSWORD "123456789" 
#define DEFAULT_SSID "ESP32_NAT_Router +"

    bool IsLedEnable, IsWebServerEnable, IsCustomDnsEnable,
        IsRandomizeMacEnable, IsDarkModeEnable, IsWifiAuthFail;

    char *customDNSip, *authUsername,
        *authPass, *macAp, *dnsIP, *cache;
    char currentMAC[18];
    int max_login_attempts, lock_out_time_minutes;
    esp_err_t parms_init();

    typedef struct
    {
        char *username;
        char *password;
    } basic_auth_info_t;

    basic_auth_info_t auth_info;

#ifdef __cplusplus
}
#endif
