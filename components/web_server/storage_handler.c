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
#include <nvs.h>
#include "cmd_nvs.h"
#include "auth_handler.h"
#include "router_globals.h"
#include "request_handler.h"
#include "initialization.h"
#include "utils.h"
#include "mac_filter.h"

static const char *TAG = "url_handler/get_data_handler";

bool error = false;

//-----------------------------------------------------------------------------
esp_err_t save_settings_data_handler(httpd_req_t *req)
{

    char *query_buf;
    size_t query_buf_len;
    char param_buf[64];
    esp_err_t err;
    bool valid_query = false;

    // get the query string
    query_buf_len = httpd_req_get_url_query_len(req) + 1;
    query_buf = malloc(query_buf_len);
    if (query_buf == NULL)
    {
        return ESP_ERR_NO_MEM;
    }
    err = httpd_req_get_url_query_str(req, query_buf, query_buf_len);
    if (err != ESP_OK)
    {
        free(query_buf);
        return err;
    }

    nvs_handle handle;
    err = nvs_open(PARAM_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "NVS error");
        valid_query = false;
    }

    if (httpd_query_key_value(query_buf, "ssid", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        ssid = html_escape(strdup(param_buf));
        err = nvs_set_str(handle, "ssid", ssid);
    }
    if (httpd_query_key_value(query_buf, "password", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        passwd = html_escape(strdup(param_buf));
        err = nvs_set_str(handle, "passwd", passwd);
    }
    if (httpd_query_key_value(query_buf, "ent_username", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        ent_username = html_escape(strdup(param_buf));
        err = nvs_set_str(handle, "ent_username", ent_username);
    }
    if (httpd_query_key_value(query_buf, "ent_identity", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        ent_identity = html_escape(strdup(param_buf));
        err = nvs_set_str(handle, "ent_identity", ent_identity);
    }
    if (httpd_query_key_value(query_buf, "apSsid", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        ap_ssid = html_escape(strdup(param_buf));
        err = nvs_set_str(handle, "ap_ssid", ap_ssid);
    }

    if (httpd_query_key_value(query_buf, "apPassword", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        ap_passwd = html_escape(strdup(param_buf));
        err = nvs_set_str(handle, "ap_passwd", ap_passwd);
    }
    // if (httpd_query_key_value(query_buf, "macAp", param_buf, sizeof(param_buf)) == ESP_OK)
    // {
    //     macAp = html_escape(strdup(param_buf));
    //     err = nvs_set_str(handle, "macAp", macAp);
    // }
    if (httpd_query_key_value(query_buf, "staticIP", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        static_ip = html_escape(strdup(param_buf));
        if (is_valid_ip(static_ip))
            err = nvs_set_str(handle, "static_ip", static_ip);
        else
            ESP_LOGE(TAG, "Invalid Static IP Address");
    }
    if (httpd_query_key_value(query_buf, "subnetMask", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        subnet_mask = html_escape(strdup(param_buf));
        if (is_valid_ip(subnet_mask))
            err = nvs_set_str(handle, "subnet_mask", subnet_mask);
        else
            ESP_LOGE(TAG, "Invalid Subnet Mask Address");
    }
    if (httpd_query_key_value(query_buf, "gateWay", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        gateway_addr = html_escape(strdup(param_buf));
        if (is_valid_ip(gateway_addr))
            err = nvs_set_str(handle, "gateway_addr", gateway_addr);
        else
            ESP_LOGE(TAG, "Invalid Gateway Addr Address");
        }
    if (httpd_query_key_value(query_buf, "apIP", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        ap_ip = html_escape(strdup(param_buf));
        if (is_valid_ip(ap_ip))
            err = nvs_set_str(handle, "ap_ip", ap_ip);
        else
            ESP_LOGE(TAG, "Invalid AP IP Address");
    }

    if (httpd_query_key_value(query_buf, "dnsIP", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        customDNSip = html_escape(strdup(param_buf));
        if (strcmp(customDNSip, "default") != 0)
        {
            if (is_valid_ip(customDNSip))
                err = nvs_set_str(handle, "custom_dns_ip", customDNSip);
            else
                ESP_LOGE(TAG, "Invalid DNS IP Address");
        }
    }

    if (httpd_query_key_value(query_buf, "adminUsername", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        authUsername = html_escape(strdup(param_buf));
        err = nvs_set_str(handle, "auth_username", authUsername);
    }
    if (httpd_query_key_value(query_buf, "adminPassword", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        authPass = html_escape(strdup(param_buf));
        err = nvs_set_str(handle, "auth_password", authPass);
    }

    if (httpd_query_key_value(query_buf, "maxLoginAttempts", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        max_login_attempts = atoi(html_escape(strdup(param_buf)));
        err = nvs_set_i32(handle, "max_login_attempts", max_login_attempts);
    }

    if (httpd_query_key_value(query_buf, "blockingTimeMin", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        lock_out_time_minutes = atoi(html_escape(strdup(param_buf)));
        err = nvs_set_i32(handle, "lock_out_time_minutes", lock_out_time_minutes);
    }

    if (httpd_query_key_value(query_buf, "randMacAp", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        IsRandomizeMacEnable = str_to_bool(html_escape(strdup(param_buf)));
        err = nvs_set_i32(handle, "randomize_mac", bool_to_int(IsRandomizeMacEnable));
    }

    if (httpd_query_key_value(query_buf, "CustomDns", param_buf, sizeof(param_buf)) == ESP_OK)
    {

        IsCustomDnsEnable = str_to_bool(html_escape(strdup(param_buf)));
        err = nvs_set_i32(handle, "custom_dns", bool_to_int(IsCustomDnsEnable));
    }

    if (httpd_query_key_value(query_buf, "ledEnable", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        IsLedEnable = str_to_bool(html_escape(strdup(param_buf)));
        err = nvs_set_i32(handle, "led_enable", bool_to_int(IsLedEnable));
    }

    if (httpd_query_key_value(query_buf, "webServer", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        IsWebServerEnable = str_to_bool(html_escape(strdup(param_buf)));
        err = nvs_set_i32(handle, "web_server", bool_to_int(IsWebServerEnable));
    }
    if (httpd_query_key_value(query_buf, "darkMode", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        IsDarkModeEnable = str_to_bool(html_escape(strdup(param_buf)));
        err = nvs_set_i32(handle, "dark_mode", bool_to_int(IsDarkModeEnable));
    }
    if (httpd_query_key_value(query_buf, "macFilterEnable", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        IsMacFilterEnable = str_to_bool(html_escape(strdup(param_buf)));
        err = nvs_set_i32(handle, "mac_Filter", bool_to_int(IsMacFilterEnable));
        printf("Commit failed! (%s)\n", esp_err_to_name(err));

    }
    ///////////////////
    if (httpd_query_key_value(query_buf, "add_mac_address", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        store_mac_address_in_nvs(html_escape(strdup(param_buf)));
        refresh_mac_filter();

    }
    
    if (httpd_query_key_value(query_buf, "remove_mac_address", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        remove_mac_address_from_nvs(html_escape(strdup(param_buf)));
        refresh_mac_filter();
    }
    
    if (httpd_query_key_value(query_buf, "filter_list_type", param_buf, sizeof(param_buf)) == ESP_OK)
    {
        IsAllowList = word_check(html_escape(strdup(param_buf)),"Allow");
        err = nvs_set_i32(handle, "Is_allow_list", bool_to_int(IsAllowList));
        refresh_mac_filter();
    }


/////////////////

    valid_query = (err == ESP_OK);

    if (valid_query)
    {
        err = nvs_commit(handle);
        if (err != ESP_OK)
        {
            free(query_buf);
            nvs_close(handle);
            return err;
        }
        nvs_close(handle);
        free(query_buf);
        return httpd_resp_send(req, "true", HTTPD_RESP_USE_STRLEN);
    }
    else
    {
        nvs_close(handle);
        free(query_buf);
        return httpd_resp_send(req, "false", HTTPD_RESP_USE_STRLEN);
    }
}

//-----------------------------------------------------------------------------