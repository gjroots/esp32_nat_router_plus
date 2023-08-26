/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */
#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include "initialization.h"
#include "router_globals.h"
#include "mac_generator.h"

static const char *TAG = "utils/initialization";

bool IsLedEnable = true, IsWebServerEnable = true,
     IsCustomDnsEnable = false, IsRandomizeMacEnable = false,
     IsDarkModeEnable = false, IsWifiAuthFail = false;

char *ssid, *ent_username, *ent_identity,
    *passwd, *static_ip, *subnet_mask, *gateway_addr,
    *ap_ssid, *ap_passwd, *ap_ip, *customDNSip, *authUsername,
    *authPass, *macAp, *dnsIP = "";

char currentMAC[18];

//-----------------------------------------------------------------------------
esp_err_t IRAM_ATTR parms_init()
{
    int webServer = 1, ledEnable = 1, customDnsEnable = 0,
        darkModeEnable = 0, randomizeMac = 0;

    // ESP_LOGW(TAG, "initialization Started");
    get_config_param_str("ssid", &ssid);
    get_config_param_str("passwd", &passwd);
    get_config_param_str("ent_username", &ent_username);
    get_config_param_str("ent_identity", &ent_identity);
    get_config_param_str("ap_ssid", &ap_ssid);
    get_config_param_str("ap_passwd", &ap_passwd);
    get_config_param_str("static_ip", &static_ip);
    get_config_param_str("subnet_mask", &subnet_mask);
    get_config_param_str("gateway_addr", &gateway_addr);
    get_config_param_str("custom_dns_ip", &customDNSip);
    get_config_param_str("ap_ip", &ap_ip);
    get_config_param_str("auth_username", &authUsername);
    // get_config_param_str("mac_ap", &macAp);
    get_config_param_str("auth_password", &authPass);

    get_config_param_int("web_server", &webServer);
    get_config_param_int("led_enable", &ledEnable);
    get_config_param_int("custom_dns", &customDnsEnable);
    get_config_param_int("dark_mode", &darkModeEnable);
    get_config_param_int("randomize_mac", &randomizeMac);
    get_config_param_int("max_login_attempts", &max_login_attempts);
    get_config_param_int("lock_out_time_minutes", &lock_out_time_minutes);

    ap_ssid = (ap_ssid != NULL) ? ap_ssid : DEFAULT_SSID;
    ap_passwd = (ap_passwd != NULL) ? ap_passwd : "";
    ssid = (ssid != NULL) ? ssid : "";
    passwd = (passwd != NULL) ? passwd : "";
    ent_username = (ent_username != NULL) ? ent_username : "";
    ent_identity = (ent_identity != NULL) ? ent_identity : "";

    static_ip = (static_ip != NULL) ? static_ip : "";
    subnet_mask = (subnet_mask != NULL) ? subnet_mask : "";
    gateway_addr = (gateway_addr != NULL) ? gateway_addr : "";

    ap_ip = (ap_ip != NULL) ? ap_ip : DEFAULT_AP_IP;
    customDNSip = (customDNSip != NULL) ? customDNSip : "default";

    authUsername = (authUsername != NULL) ? authUsername : DEFAULT_ADMIN_USERNAME;
    authPass = (authPass != NULL) ? authPass : DEFAULT_ADMIN_PASSWORD;
    max_login_attempts = (max_login_attempts <= 0) ? MAX_LOGIN_ATTEMPTS : max_login_attempts;
    lock_out_time_minutes = (lock_out_time_minutes <= 0) ? LOCK_OUT_TIME_MINUTES : lock_out_time_minutes;

    IsWebServerEnable = (webServer != 0);
    IsLedEnable = (ledEnable != 0);
    IsCustomDnsEnable = (customDnsEnable != 0);
    IsDarkModeEnable = (darkModeEnable != 0);
    IsRandomizeMacEnable = (randomizeMac != 0);
    auth_info.username = authUsername;
    auth_info.password = authPass;

    if (IsCustomDnsEnable)
    {
        if (strcmp(customDNSip, "default") == 0)
        {
            customDNSip = DEFAULT_DNS1;
        }
    }

    macAp = currentMAC; //(macAp != NULL) ? macAp : currentMAC;

    ESP_LOGI(TAG, "Initialization complected");
    return ESP_OK;
}
