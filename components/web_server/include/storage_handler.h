/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_err.h"
#include <esp_http_server.h>

    esp_err_t save_settings_data_handler(httpd_req_t *req);

#ifdef __cplusplus
}
#endif
