/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */
/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include <esp_http_server.h>
#include "web_server.h"
#include "router_globals.h"
#include "request_handler.h"
#include "auth_handler.h"

static const char *TAG = "HTTPServer";

// declare global variable to store server handle
httpd_handle_t server = NULL;

static httpd_uri_t get_scan = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_scan_handler,
    .user_ctx = &auth_info};

static httpd_uri_t get_settings = {
    .uri = "/settings",
    .method = HTTP_GET,
    .handler = get_settings_handler,
    .user_ctx = &auth_info};

static httpd_uri_t get_info = {
    .uri = "/info",
    .method = HTTP_GET,
    .handler = get_info_handler,
    .user_ctx = &auth_info};

static httpd_uri_t get_main_css = {
    .uri = "/main.css",
    .method = HTTP_GET,
    .handler = get_main_css_handler,
    .user_ctx = &auth_info};

static httpd_uri_t get_dark_css = {
    .uri = "/dark.css",
    .method = HTTP_GET,
    .handler = get_dark_css_handler,
    .user_ctx = &auth_info};

static httpd_uri_t get_error_404 = {
    .uri = "/404",
    .method = HTTP_GET,
    .handler = get_error_404_handler,
    .user_ctx = &auth_info};

// javascript
static httpd_uri_t common_js_path = {
    .uri = "/js/*",
    .method = HTTP_GET,
    .handler = common_js_path_handler,
    .user_ctx = &auth_info};

static httpd_uri_t common_data_path = {
    .uri = "/data/*",
    .method = HTTP_GET,
    .handler = common_data_handler,
    .user_ctx = &auth_info};

static httpd_uri_t settings_post_path = {
    .uri = "/data/settingsSave.json",
    .method = HTTP_POST,
    .handler = common_data_handler,
    .user_ctx = &auth_info};

static httpd_uri_t post_ota_update = {
    .uri = "/ota",
    .method = HTTP_POST,
    .handler = post_ota_update_handler,
    .user_ctx = &auth_info};

//-----------------------------------------------------------------------------
esp_err_t http_404_error_handler(httpd_req_t *req)
{
    httpd_resp_set_status(req, "302 Temporary Redirect");
    httpd_resp_set_hdr(req, "Location", "/404");
    httpd_resp_send(req, "Redirect to 404 page", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static httpd_uri_t error_404_handler = {
    .uri = "/*",
    .method = HTTP_GET,
    .handler = http_404_error_handler,
    .user_ctx = NULL};

//-----------------------------------------------------------------------------
httpd_handle_t start_webserver(void)
{
    // httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    // config.task_priority = 6,
    // config.max_resp_headers = 1024;
    config.max_uri_handlers = 15;
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.lru_purge_enable = true;

    // config.stack_size = 10000;

    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error starting server!");
        return NULL;
    }
    ESP_LOGI(TAG, "Registering URI handlers");
    httpd_register_uri_handler(server, &get_scan);
    httpd_register_uri_handler(server, &get_info);
    httpd_register_uri_handler(server, &get_settings);
    httpd_register_uri_handler(server, &get_main_css);
    httpd_register_uri_handler(server, &get_dark_css);
    httpd_register_uri_handler(server, &common_js_path);
    httpd_register_uri_handler(server, &common_data_path);
    httpd_register_uri_handler(server, &settings_post_path);
    httpd_register_uri_handler(server, &post_ota_update);
    httpd_register_uri_handler(server, &get_error_404);
    httpd_register_uri_handler(server, &error_404_handler);
    return server;
}

//-----------------------------------------------------------------------------
void stop_web_server(void)
{
    if (server != NULL)
    { // check if server handle is valid
        httpd_stop(server);
        server = NULL; // reset server handle to NULL
        ESP_LOGI(TAG, "Http server stoped");
    }
}

//-----------------------------------------------------------------------------
void toggle_webserver(void)
{
    IsWebServerEnable = !IsWebServerEnable;
    if (IsWebServerEnable)
    {
        server = start_webserver(); // store server handle in global variable
    }
    else
    {
        stop_web_server(); // use global variable to stop server
    }
}

//-----------------------------------------------------------------------------