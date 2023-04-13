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
#include "auth_handler.h"
#include "request_handler.h"
// static const char *TAG = "components/uurl_request_handler";
#include <esp_log.h>

/* ********   Request Handlers  ************ */

//-----------------------------------------------------------------------------
esp_err_t get_scan_handler(httpd_req_t *req)
{
    return authentication_handler(req, SCAN_PAGE_ID);
}


esp_err_t get_settings_handler(httpd_req_t *req)
{
    return authentication_handler(req, SETTINGS_PAGE_ID);
}


esp_err_t get_info_handler(httpd_req_t *req)
{
    return authentication_handler(req, INFO_PAGE_ID);
}


//-----------------------------------------------------------------------------
esp_err_t get_main_css_handler(httpd_req_t *req)
{
    return common_page_response_handler(req, MAIN_CSS_ID);
}
esp_err_t get_dark_css_handler(httpd_req_t *req)
{
    return common_page_response_handler(req, DARK_CSS_ID);
}


//-----------------------------------------------------------------------------
esp_err_t common_js_path_handler(httpd_req_t *req)
{
    if (strcmp(req->uri, "/js/functions.js") == 0)
    {
        return common_js_response(req);
    }
    return authentication_handler(req, COMMON_JS_ID);
}


//-----------------------------------------------------------------------------
esp_err_t common_data_handler(httpd_req_t *req)
{
    return authentication_handler(req, COMMON_DATA_ID);
}


//-----------------------------------------------------------------------------
esp_err_t post_ota_update_handler(httpd_req_t *req)
{
    return authentication_handler(req, POST_OTA_UPDATE_ID);
}


//-----------------------------------------------------------------------------
esp_err_t get_error_404_handler(httpd_req_t *req)
{
    return common_page_response_handler(req, ERR_404_PAGE_ID);
}