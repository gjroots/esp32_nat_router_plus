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

#include <esp_http_server.h>
#include "initialization.h"

    esp_err_t authentication_handler(httpd_req_t *req, int url_id_number);
    extern int max_login_attempts;
    extern int lock_out_time_minutes;

#ifdef __cplusplus
}
#endif
