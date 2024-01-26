/**
 * @author Jaya Satish
 * @license
 * this software licensed under MIT
 *
 *
 */

#include <stdio.h>
#include <nvs_flash.h>
#include <cJSON.h>
#include <string.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_netif.h>
#include <esp_wifi_types.h>

#include "mac_filter.h"
#include "initialization.h"


bool check_mac_address_in_nvs(const char *macAddress) {
    cJSON *json = retrieve_mac_addresses_as_json();
    if (json == NULL) return false; 

    int numMacs = cJSON_GetArraySize(json);
    for (int i = 0; i < numMacs; i++) {
        if (strcmp(cJSON_GetArrayItem(json, i)->valuestring, macAddress) == 0) {
            cJSON_Delete(json); 
            return true; 
        }
    }

    cJSON_Delete(json); 
    return false;
}

//-----------------------------------------------------------------------------
void mac_filter(const uint8_t mac[6], uint8_t aid) {
    char mac_address[18];
    sprintf(mac_address, MACSTR, MAC2STR(mac));
    if (IsMacFilterEnable) {
        if ((IsAllowList && !check_mac_address_in_nvs(mac_address)) || 
            (!IsAllowList && check_mac_address_in_nvs(mac_address))) {
            ESP_LOGI("MAC Filter","MAC Address %s not allowed, disconnecting...\n", IsAllowList ? "NOT found in Allow List" : "found in Deny List");
            esp_wifi_deauth_sta(aid);
        }
    }
}

//-----------------------------------------------------------------------------
esp_err_t check_in_filter_list(const uint8_t mac[6]) {
    uint16_t aid;
    esp_err_t err = esp_wifi_ap_get_sta_aid(mac, &aid);
    if (err == ESP_OK) {
        mac_filter(mac, aid);
    } else {
        printf("Failed to get AID for MAC address\n");
    }
    return err;
}

//-----------------------------------------------------------------------------
void refresh_mac_filter() {
    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;
    memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
    memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
    ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list(&wifi_sta_list));
    ESP_ERROR_CHECK(tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list));

    for (int i = 0; i < adapter_sta_list.num; i++) {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        esp_err_t result = check_in_filter_list(station.mac);
        if (result != ESP_OK) {
            //printf("Error processing MAC address at index %d\n", i);
        }

    }
}

//-----------------------------------------------------------------------------