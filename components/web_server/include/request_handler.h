/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */

#ifndef URL_REQUEST_HANDLER
#define URL_REQUEST_HANDLER

#endif

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_http_server.h"

#define SCAN_PAGE_ID 1
#define SETTINGS_PAGE_ID 2
#define INFO_PAGE_ID 3
#define MAIN_CSS_ID 4
#define DARK_CSS_ID 5

#define COMMON_JS_ID 6
#define COMMON_DATA_ID 7

#define POST_OTA_UPDATE_ID 8
#define ERR_404_PAGE_ID 404

    // Register NVS functions
    esp_err_t get_scan_handler(httpd_req_t *req);
    esp_err_t get_settings_handler(httpd_req_t *req);

    esp_err_t get_info_handler(httpd_req_t *req);
    esp_err_t get_main_css_handler(httpd_req_t *req);
    esp_err_t get_dark_css_handler(httpd_req_t *req);
    esp_err_t get_error_404_handler(httpd_req_t *req);

    esp_err_t common_page_response_handler(httpd_req_t *req, int req_id);
    esp_err_t common_js_path_handler(httpd_req_t *req);
    esp_err_t common_js_response(httpd_req_t *req);

    esp_err_t common_data_handler(httpd_req_t *req);
    esp_err_t common_data_request(httpd_req_t *req);

    esp_err_t http_404_error_handler(httpd_req_t *req);

    esp_err_t get_settings_data_handler(httpd_req_t *req);
    esp_err_t save_settings_data_handler(httpd_req_t *req);

    esp_err_t get_scan_data_handler(httpd_req_t *req);
    esp_err_t get_info_data_handler(httpd_req_t *req);

    // esp_err_t ssids_post_handler(httpd_req_t *req);
    esp_err_t post_ota_update_handler(httpd_req_t *req);

#ifdef __cplusplus
}
#endif
