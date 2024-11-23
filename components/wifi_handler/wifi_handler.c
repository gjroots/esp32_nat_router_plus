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
#include <esp_wifi.h>
#include <esp_netif.h>
#include <esp_wifi_types.h>
#include <cJSON.h>
#include "router_globals.h"
#include "initialization.h"
#include "wifi_handler.h"
#include "wifi_event_handler.h"
#include "mac_filter.h"
#include "esp_mac.h"
#include "esp_wifi_ap_get_sta_list.h"

bool is_scanning_progress = false;
//-----------------------------------------------------------------------------
// Handles wifi scan and return AP records in json string format
char* IRAM_ATTR wifi_scan_handler(void)
{
    // Disconnect from the current access point if not already connected
    if (!ap_connect)
    {
        is_scanning_progress = true;
        esp_wifi_disconnect();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
    };
    printf("Start scanning...\n");
    esp_err_t err = esp_wifi_scan_start(&scan_config, true);
    if (err == ESP_OK)
    {
        printf("scanning completed!\n");
        uint16_t ap_num;
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_num));
        wifi_ap_record_t *ap_records = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * 100);
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));
        printf("Found %d access points:\n", ap_num);
        cJSON *root = cJSON_CreateObject();
        cJSON *aps = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "aps", aps);
        char bssid_str[18];
        for (int i = 0; i < ap_num; i++)
        {
            sprintf(bssid_str, MACSTR, MAC2STR(ap_records[i].bssid));
            cJSON *ap = cJSON_CreateObject();
            cJSON_AddNumberToObject(ap, "c", ap_records[i].primary);
            cJSON_AddStringToObject(ap, "m", (const char *)bssid_str);
            cJSON_AddStringToObject(ap, "ss", (const char *)ap_records[i].ssid);
            cJSON_AddNumberToObject(ap, "r", ap_records[i].rssi);
            cJSON_AddNumberToObject(ap, "e", ap_records[i].authmode);
            cJSON_AddItemToArray(aps, ap);
        }
        char *my_json_string = cJSON_Print(root);
        cJSON_Delete(root);
        free(ap_records);
        // Reconnect to the access point if not already connected
        if (!ap_connect)
        {
            is_scanning_progress = false;
            vTaskDelay(300 / portTICK_PERIOD_MS);
            esp_wifi_connect();
        }
        return my_json_string;
    }
    printf("scanning Failed!\n");
    return "false";
}

//-----------------------------------------------------------------------------
// Handles wifi information and return in json format
char* IRAM_ATTR wifi_info_handler(void)
{
    wifi_ap_record_t ap_info;
    wifi_sta_list_t wifi_sta_list;
    wifi_sta_mac_ip_list_t adapter_sta_list;
    esp_netif_ip_info_t ip_info;
    esp_netif_dns_info_t dns_info;
    char *ssid = "";
    int8_t rssi = 0;
    char gateway_address[32];
    char ip_address[32];
    char dns[32];

    if (ap_connect) {
        memset(&ap_info, 0, sizeof(ap_info));
        memset(&ip_info, 0, sizeof(ip_info));
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            ssid = (char *)ap_info.ssid;
            rssi = ap_info.rssi;
            ESP_ERROR_CHECK(esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info));
            ESP_ERROR_CHECK(esp_netif_get_dns_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), ESP_NETIF_DNS_MAIN, &dns_info));

            strlcpy(gateway_address, ip4addr_ntoa((const ip4_addr_t *)&ip_info.gw), sizeof(gateway_address));
            strlcpy(ip_address, ip4addr_ntoa((const ip4_addr_t *)&ip_info.ip), sizeof(ip_address));
            strlcpy(dns, ip4addr_ntoa((const ip4_addr_t *)&dns_info.ip), sizeof(dns));
        } else {
            ssid = "";
            rssi = 0;
            strcpy(gateway_address, "");
            strcpy(ip_address, "");
            strcpy(dns, "");
        }
    }
    memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
    memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
    ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list(&wifi_sta_list));
    ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list_with_ip(&wifi_sta_list, &adapter_sta_list));

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "ssid", ssid);
    cJSON_AddStringToObject(root, "gatewayAddress", gateway_address);
    cJSON_AddStringToObject(root, "ipAddress", ip_address);
    cJSON_AddStringToObject(root, "dns", (has_static_ip || IsCustomDnsEnable) ? customDNSip : dns);
    cJSON_AddStringToObject(root, "filterListType", (IsAllowList ? "Allow" : "Deny"));
    cJSON_AddNumberToObject(root, "rss", rssi);
    cJSON_AddBoolToObject(root, "wifiAuthFail", IsWifiAuthFail);
    cJSON *clients = cJSON_AddArrayToObject(root, "clients");
    cJSON *json = retrieve_mac_addresses_as_json();

    // Iterate over the connected stations
    for (int i = 0; i < wifi_sta_list.num; i++) {
        esp_netif_pair_mac_ip_t station = adapter_sta_list.sta[i];
        cJSON *client = cJSON_CreateObject();
        cJSON_AddStringToObject(client, "ipAddress", ip4addr_ntoa((const ip4_addr_t *)&(station.ip)));
        char mac_address[18];
        sprintf(mac_address, MACSTR, MAC2STR(station.mac));
        cJSON_AddStringToObject(client, "macAddress", mac_address);
        cJSON_AddItemToArray(clients, client);
        
    }
    
    if (json != NULL) {
        cJSON_AddItemToObject(root, "filterList", json);
    }
    char *my_json_string = cJSON_Print(root);
    cJSON_Delete(root);
    return my_json_string;
}

//-----------------------------------------------------------------------------