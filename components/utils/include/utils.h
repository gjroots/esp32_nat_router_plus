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

#include <stdint.h>
#include <stdlib.h>
#include <esp_err.h>

#ifdef ICACHE_FLASH
#define ICACHE_FLASH_ATTR __attribute__((section(".irom0.text")))
#define ICACHE_RODATA_ATTR __attribute__((section(".irom.text")))
#else
#define ICACHE_FLASH_ATTR
#define ICACHE_RODATA_ATTR
#endif /* ICACHE_FLASH */

    float system_uptime_s(void);
    void restart(void);

    bool is_valid_ip(const char *ip_str);
    bool str_to_bool(const char *key);
    int bool_to_int(bool value);
    bool word_check(const char *str, const char *word);
    char *html_escape(const char *src);

#ifdef __cplusplus
}
#endif
