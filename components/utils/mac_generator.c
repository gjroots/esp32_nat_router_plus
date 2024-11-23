/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <esp_system.h>
#include <esp_log.h>
#include "utils.h"
#include "esp_efuse.h" 
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_netif.h" 
#include "esp_wifi.h"

#include "router_globals.h"
#include "initialization.h"
#include "mac_generator.h"

static const char *TAG = "utils/MAC Generator";

bool esp_base_mac_addr_check(uint8_t *mac)
{
    // Check if MAC address is all zeros or all ones
    for (int i = 0; i < 6; i++)
    {
        if (mac[i] != 0x00 && mac[i] != 0xff)
        {
            return false;
        }
    }
    return true;
}

void custom_mac_generator(void)
{
    uint8_t default_mac_addr[6] = {0};
    esp_err_t err = esp_efuse_mac_get_default(default_mac_addr);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read default MAC address");
        return;
    }
    if (IsRandomizeMacEnable)
    {
        uint8_t random_mac_addr[6];
        bool is_valid_mac = false;
        memcpy(random_mac_addr, default_mac_addr, sizeof(default_mac_addr));
        while (!is_valid_mac)
        {
            for (int i = 1; i < 6; i++)
            {
                random_mac_addr[i] = esp_random() % 256;
            }
            // Check if the random MAC address is already in use
            if (!esp_base_mac_addr_check(random_mac_addr))
            {
                is_valid_mac = true;
            }
        }
        // Set the new MAC address with the first byte unchanged
        err = esp_base_mac_addr_set(random_mac_addr);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to set random MAC address");
            return;
        }
        uint8_t read_mac_addr[6] = {0};
        err = esp_base_mac_addr_get(read_mac_addr);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to read MAC address");
            return;
        }
        if (memcmp(read_mac_addr, random_mac_addr, 6) == 0)
        {
            sprintf(currentMAC, MACSTR, MAC2STR(random_mac_addr));
        }
    }
    else
    {
        sprintf(currentMAC, MACSTR, MAC2STR(default_mac_addr));
    }
    ESP_LOGI(TAG, "Setting random MAC address: %s", currentMAC);
}
