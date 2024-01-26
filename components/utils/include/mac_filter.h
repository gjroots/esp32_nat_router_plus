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
#include <stdbool.h>
#include <esp_err.h>
#include <cJSON.h>

#define MAX_MAC_ADDRESSES 10
#define NVS_KEY "stored_macs"


esp_err_t store_mac_address_in_nvs(const char *macAddress);
esp_err_t remove_mac_address_from_nvs(const char *macAddress);
bool check_mac_address_in_nvs(const char *macAddress);
cJSON *retrieve_mac_addresses_as_json();
void mac_filter(const uint8_t mac[6],uint8_t aid);
void refresh_mac_filter();

#ifdef __cplusplus
}
#endif
