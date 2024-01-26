/**
 * @author Jaya Satish
 * 
 *@copyright Copyright (c) 2023
 *Licensed under MIT 
 *
 */
#include <stdio.h>
#include <string.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include "esp_err.h"
#include <cJSON.h>

#include "nvm.h"
#include "router_globals.h"
#include "utils.h"
#include "mac_filter.h"

static const char *TAG = "utils/nvm";

void initialize_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

//-----------------------------------------------------------------------------
esp_err_t nvm_set_bool(const char *key, bool value)
{

    nvs_handle handle;
    error_t err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "NVS open error");
        return ESP_FAIL;
    }
    return nvs_set_i32(handle, key, bool_to_int(value));
}

//-----------------------------------------------------------------------------
esp_err_t nvm_erase(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "NVS open error");
        return ESP_FAIL;
    }
    err = nvs_erase_all(nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "NVS Erase all error");
        return ESP_FAIL;
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK)
    {
        return ESP_FAIL;
    }
    nvs_close(nvs_handle);
    return ESP_OK;
}

//-----------------------------------------------------------------------------
cJSON *retrieve_mac_addresses_as_json() {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(PARAM_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return NULL;
    }

    size_t required_size;
    err = nvs_get_str(handle, NVS_KEY, NULL, &required_size);
    if (err != ESP_OK || required_size == 0) {
        nvs_close(handle);
        return NULL;
    }

    char *buffer = malloc(required_size);
    if (buffer == NULL) {
        nvs_close(handle);
        return NULL;
    }

    err = nvs_get_str(handle, NVS_KEY, buffer, &required_size);
    if (err != ESP_OK) {
        free(buffer);
        nvs_close(handle);
        return NULL;
    }

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);
    nvs_close(handle);
    return json;
}

//-----------------------------------------------------------------------------
esp_err_t save_mac_addresses_as_json(cJSON *json) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        cJSON_Delete(json);
        return err;
    }

    char *serialized = cJSON_Print(json);
    if (serialized == NULL) {
        cJSON_Delete(json);
        nvs_close(handle);
        return ESP_ERR_NO_MEM;
    }

    err = nvs_set_str(handle, NVS_KEY, serialized);

    ESP_LOGI("MACS ARE","%s",serialized);
    free(serialized);
    nvs_close(handle);
    return err;
}

//-----------------------------------------------------------------------------
esp_err_t store_mac_address_in_nvs(const char *macAddress) {
    cJSON *json = retrieve_mac_addresses_as_json();
    if (json == NULL) {
        json = cJSON_CreateArray();
    }

    int numMacs = cJSON_GetArraySize(json);
    if (numMacs >= MAX_MAC_ADDRESSES) {
        cJSON_Delete(json);
        return ESP_ERR_NO_MEM; 
    }

    for (int i = 0; i < numMacs; i++) {
        const char *storedMac = cJSON_GetArrayItem(json, i)->valuestring;
        if (strcmp(storedMac, macAddress) == 0) {
            cJSON_Delete(json);
            return ESP_OK; 
        }
    }

    cJSON_AddItemToArray(json, cJSON_CreateString(macAddress));

    esp_err_t err = save_mac_addresses_as_json(json);
    cJSON_Delete(json); 
    return err;
}

//-----------------------------------------------------------------------------
esp_err_t remove_mac_address_from_nvs(const char *macAddress) {
    cJSON *json = retrieve_mac_addresses_as_json();
    if (json == NULL) {
        return ESP_ERR_NVS_NOT_FOUND; 
    }

    int numMacs = cJSON_GetArraySize(json);
    for (int i = 0; i < numMacs; i++) {
        const char *storedMac = cJSON_GetArrayItem(json, i)->valuestring;
        if (strcmp(storedMac, macAddress) == 0) {
            cJSON_DeleteItemFromArray(json, i);

            esp_err_t err = save_mac_addresses_as_json(json);
            cJSON_Delete(json); 
            return err;
        }
    }
    cJSON_Delete(json); 
    return ESP_ERR_NOT_FOUND; 
}

//-----------------------------------------------------------------------------