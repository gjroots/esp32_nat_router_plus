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
#include "nvm.h"
#include "router_globals.h"
#include "utils.h"

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
