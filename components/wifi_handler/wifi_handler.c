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
        vTaskDelay(500 / portTICK_RATE_MS);
    }

    // Configure the scan parameters
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
        // Get the number of access points found

        uint16_t ap_num;
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_num));
        // Allocate memory for the list of access points
        wifi_ap_record_t *ap_records = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * 100);
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_num, ap_records));
        printf("Found %d access points:\n", ap_num);
        // Create a JSON object to store the list of access points
        cJSON *root = cJSON_CreateObject();
        cJSON *aps = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "aps", aps);
        char bssid_str[18];
        for (int i = 0; i < ap_num; i++)
        {
            sprintf(bssid_str, MACSTR, MAC2STR(ap_records[i].bssid));
            // Create a JSON object for each access point
            cJSON *ap = cJSON_CreateObject();
            cJSON_AddNumberToObject(ap, "c", ap_records[i].primary);
            cJSON_AddStringToObject(ap, "m", (const char *)bssid_str);
            cJSON_AddStringToObject(ap, "ss", (const char *)ap_records[i].ssid);
            cJSON_AddNumberToObject(ap, "r", ap_records[i].rssi);
            cJSON_AddNumberToObject(ap, "e", ap_records[i].authmode);
            cJSON_AddItemToArray(aps, ap);
        }
        // Convert the JSON object to a string
        char *my_json_string = cJSON_Print(root);
        cJSON_Delete(root);
        free(ap_records);
        // Reconnect to the access point if not already connected
        if (!ap_connect)
        {
            is_scanning_progress = false;
            vTaskDelay(300 / portTICK_RATE_MS);
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
    tcpip_adapter_sta_list_t adapter_sta_list;
    tcpip_adapter_ip_info_t ip_info;
    tcpip_adapter_dns_info_t dns_info;
    int8_t rssi = 0;
    char *dns = "";
    char *ip_address = "";
    memset(&ap_info, 0, sizeof(ap_info));
    // Get the access point info if connected
    if (ap_connect)
    {
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK)
        {
            rssi = ap_info.rssi;
            tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
            ip_address = ip4addr_ntoa(&ip_info.ip);
            tcpip_adapter_get_dns_info(TCPIP_ADAPTER_IF_STA, ESP_NETIF_DNS_MAIN, &dns_info);
            dns = ip4addr_ntoa((ip4_addr_t *)&dns_info.ip);
        }
        else
        {
            rssi = 0;
            dns = "";
            ip_address = "";
        }
    }
    // Get the list of connected clients
    memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
    memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
    ESP_ERROR_CHECK(esp_wifi_ap_get_sta_list(&wifi_sta_list));
    ESP_ERROR_CHECK(tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list));
    // Create a JSON object to store the access point and client info
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "ipAddress", ip_address);
    cJSON_AddStringToObject(root, "gatewayAddress", dns);
    cJSON_AddNumberToObject(root, "rss", rssi);
    cJSON_AddBoolToObject(root, "wifiAuthFail", IsWifiAuthFail);
    cJSON *clients = cJSON_AddArrayToObject(root, "clients");
    for (int i = 0; i < adapter_sta_list.num; i++)
    {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        // Create a JSON object for each connected client
        cJSON *client = cJSON_CreateObject();
        cJSON_AddStringToObject(client, "ipAddress", ip4addr_ntoa((ip4_addr_t *)&(station.ip)));
        char mac_address[18];
        sprintf(mac_address, MACSTR, MAC2STR(station.mac));
        cJSON_AddStringToObject(client, "macAddress", mac_address);
        cJSON_AddItemToArray(clients, client);
    }
    // Convert the JSON object to a string
    char *my_json_string = cJSON_Print(root);
    cJSON_Delete(root);
    return my_json_string;
}

//-----------------------------------------------------------------------------