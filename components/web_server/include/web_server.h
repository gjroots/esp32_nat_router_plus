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

    void stop_web_server(void);
    void toggle_webserver(void);
    extern httpd_handle_t server;
    httpd_handle_t start_webserver(void);

#ifdef __cplusplus
}
#endif
