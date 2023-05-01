/**
 * @author Jaya Satish
 * 
 *@copyright Copyright (c) 2023
 *Licensed under MIT 
 *
 */
#include <stdio.h>
#include <string.h>
#include "esp_http_server.h"
#include <esp_log.h>
#include "router_globals.h"
#include "request_handler.h"
#include "initialization.h"
#include "storage_handler.h"
#include "utils.h"
#include "nvm.h"
static const char *TAG = "components/url_requesr_handler";

// it is possible to simplify the code by using a macro that specifies the symbol name.

// <<<--- it is equivalent to --->>>
// extern const char name_start[] asm("_binary_name_start");
// extern const char name_end[] asm("_binary_name_end");
// #define HTTPD_404 404_html_gz_start

#define BINARY_SYMBOL(name)                                          \
    extern const char name##_start[] asm("_binary_" #name "_start"); \
    extern const char name##_end[] asm("_binary_" #name "_end");

BINARY_SYMBOL(index_html_gz);
BINARY_SYMBOL(settings_html_gz);
BINARY_SYMBOL(info_html_gz);
BINARY_SYMBOL(error_404_html_gz);

BINARY_SYMBOL(main_css_gz);
BINARY_SYMBOL(dark_css_gz);

// javaScript
BINARY_SYMBOL(scan_js_gz);
BINARY_SYMBOL(info_js_gz);
BINARY_SYMBOL(settings_js_gz);
BINARY_SYMBOL(functions_js_gz);
BINARY_SYMBOL(info_js_gz);

/************* returns subsequent Response pages***************/
typedef struct
{
    const int id;
    const char *type;
    const uint8_t *start;
    const uint8_t *end;
} url_handler_t;

url_handler_t handlers[] = {
    {SCAN_PAGE_ID, "text/html", (const uint8_t *)index_html_gz_start, (const uint8_t *)index_html_gz_end},
    {SETTINGS_PAGE_ID, "text/html", (const uint8_t *)settings_html_gz_start, (const uint8_t *)settings_html_gz_end},
    {INFO_PAGE_ID, "text/html", (const uint8_t *)info_html_gz_start, (const uint8_t *)info_html_gz_end},
    {ERR_404_PAGE_ID, "text/html", (const uint8_t *)error_404_html_gz_start, (const uint8_t *)error_404_html_gz_end},
    {MAIN_CSS_ID, "text/css", (const uint8_t *)main_css_gz_start, (const uint8_t *)main_css_gz_end},
    {DARK_CSS_ID, "text/css", (const uint8_t *)dark_css_gz_start, (const uint8_t *)dark_css_gz_end},
};


//-----------------------------------------------------------------------------
esp_err_t ICACHE_FLASH_ATTR common_page_response_handler(httpd_req_t *req, int req_id)
{
    for (int i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++)
    {
        if (handlers[i].id == req_id)
        {
            if (!IsDarkModeEnable && req_id == DARK_CSS_ID)
            {
                httpd_resp_set_type(req, handlers[i].type);
                return httpd_resp_send(req, "/* Dark mode disabled */", HTTPD_RESP_USE_STRLEN);
            }
            if (req_id == ERR_404_PAGE_ID)
            {
                httpd_resp_set_status(req, HTTPD_404);
            }

            const size_t handler_size = handlers[i].end - handlers[i].start;
            httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
            return httpd_resp_send(req, (const char *)handlers[i].start, handler_size);
        }
    }
    httpd_resp_send_404(req);
    return ESP_OK;
}

/////////////////////////////////////////**************JAVA_SCRIPT*********///////////////////////////////////////////////////////

typedef struct
{
    const char *uri;
    const uint8_t *start;
    const uint8_t *end;
} js_file_t;

js_file_t js_files[] = {
    {"/js/scan.js", (const uint8_t *)scan_js_gz_start, (const uint8_t *)scan_js_gz_end},
    {"/js/info.js", (const uint8_t *)info_js_gz_start, (const uint8_t *)info_js_gz_end},
    {"/js/settings.js", (const uint8_t *)settings_js_gz_start, (const uint8_t *)settings_js_gz_end},
    {"/js/functions.js", (const uint8_t *)functions_js_gz_start, (const uint8_t *)functions_js_gz_end},
    // {"/js/site.js", (const uint8_t *)site_js_gz_start, (const uint8_t *)site_js_gz_end}
};
const size_t num_js_files = sizeof(js_files) / sizeof(js_files[0]);


//-----------------------------------------------------------------------------
esp_err_t ICACHE_FLASH_ATTR common_js_response(httpd_req_t *req)
{
    for (int i = 0; i < num_js_files; i++)
    {
        if (strcmp(req->uri, js_files[i].uri) == 0)
        {
            const size_t js_size = (js_files[i].end - js_files[i].start);
            httpd_resp_set_hdr(req, "Content-Encoding", "gzip");
            httpd_resp_set_type(req, "application/javascript");

            return httpd_resp_send(req, (const char *)js_files[i].start, js_size);
        }
    }
    return http_404_error_handler(req);
}

//-----------------------------------------------------------------------------
esp_err_t common_data_request(httpd_req_t *req)
{

    if (strcmp(req->uri, "/data/settings.json") == 0)
    {
        return get_settings_data_handler(req);
    }
    else if (word_check(req->uri, "/data/settingsSave.json"))
    {
        return save_settings_data_handler(req);
    }

    else if (strcmp(req->uri, "/data/APScanResults.json") == 0)
    {
        return get_scan_data_handler(req);
    }
    else if (strcmp(req->uri, "/data/sysinfo.json") == 0)
    {
        return get_info_data_handler(req);
    }
    else if (strcmp(req->uri, "/data/ClientScanTime.json") == 0)
    {
        httpd_resp_set_type(req, "application/json");
        return httpd_resp_send(req, "true", HTTPD_RESP_USE_STRLEN);
    }
    else if (strcmp(req->uri, "/data/restart.json") == 0)
    {
        ESP_LOGW(TAG, "Restarting now...");
        httpd_resp_set_type(req, "application/json");
        if (httpd_resp_send(req, "true", HTTPD_RESP_USE_STRLEN) == ESP_OK)
        {
            vTaskDelay(pdMS_TO_TICKS(2000)); // delay for 3 seconds
            restart();
        }
        return ESP_OK;
    }
    else if (strcmp(req->uri, "/data/settingsReset.json") == 0)
    {

        if (nvm_erase() == ESP_OK)
            httpd_resp_send(req, "true", HTTPD_RESP_USE_STRLEN);

        else
            return httpd_resp_send(req, "false", HTTPD_RESP_USE_STRLEN);
    }

    return http_404_error_handler(req);
}
