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
#include <cJSON.h>

#include "get_data_handler.h"
#include "router_globals.h"
#include "request_handler.h"
#include "auth_handler.h"
#include "initialization.h"
#include "wifi_handler.h"

#define MAX_CLIENTS 10

static const char *TAG = "Web server/get_data_handler";

//-----------------------------------------------------------------------------
esp_err_t get_settings_data_handler(httpd_req_t *req)
{
    parms_init();

    cJSON *root;
    root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "ssid", ssid);
    cJSON_AddStringToObject(root, "password", passwd);
    cJSON_AddStringToObject(root, "entUsername", ent_username);
    cJSON_AddStringToObject(root, "entIdentity", ent_identity);
    cJSON_AddStringToObject(root, "apSsid", ap_ssid);
    cJSON_AddStringToObject(root, "apPassword", ap_passwd);

    cJSON_AddStringToObject(root, "apIP", ap_ip);

    cJSON_AddStringToObject(root, "staticIP", static_ip);
    cJSON_AddStringToObject(root, "subnetMask", subnet_mask);
    cJSON_AddStringToObject(root, "gateWay", gateway_addr);

    cJSON_AddStringToObject(root, "macAp", currentMAC);
    cJSON_AddStringToObject(root, "dnsIP", customDNSip);

    cJSON_AddBoolToObject(root, "randMacAp", IsRandomizeMacEnable);
    cJSON_AddBoolToObject(root, "CustomDns", IsCustomDnsEnable);
    cJSON_AddBoolToObject(root, "ledEnable", IsLedEnable);
    cJSON_AddBoolToObject(root, "darkMode", IsDarkModeEnable);
    cJSON_AddBoolToObject(root, "webServer", IsWebServerEnable);
    cJSON_AddStringToObject(root, "authUsername", authUsername);
    cJSON_AddStringToObject(root, "authPassword", authPass);
    cJSON_AddNumberToObject(root, "maxLoginAttempts", max_login_attempts);
    cJSON_AddNumberToObject(root, "blockingTimeMin", lock_out_time_minutes);
    char *my_json_string = cJSON_Print(root);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, my_json_string, HTTPD_RESP_USE_STRLEN);
    // ESP_LOGI(TAG, "Requesting settings Data");

    cJSON_Delete(root);
    return ESP_OK;
}

//-----------------------------------------------------------------------------
esp_err_t get_scan_data_handler(httpd_req_t *req)
{

    char *scan_result = wifi_scan_handler();

    httpd_resp_set_type(req, "application/json");

    httpd_resp_send(req, scan_result, HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, "Requesting scan aps Data");
    free(scan_result);

    return ESP_OK;
}

//-----------------------------------------------------------------------------
esp_err_t get_info_data_handler(httpd_req_t *req)
{
    char *info_result = wifi_info_handler();

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, info_result, HTTPD_RESP_USE_STRLEN);
    // ESP_LOGI(TAG, "Requesting Info Data");
    free(info_result);
    return ESP_OK;
}
