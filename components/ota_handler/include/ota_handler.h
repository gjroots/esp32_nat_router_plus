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
#include <stdio.h>
#include <string.h>
#include "esp_http_server.h"

    void ota_update_init(void);
    esp_err_t ota_update_handler(httpd_req_t *req);

#ifdef __cplusplus
}
#endif