/**
 * @author Jaya Satish
 *
 *@copyright Copyright (c) 2023
 *Licensed under MIT
 *
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <esp_log.h>
#include <lwip/inet.h>
#include <lwip/sockets.h>
#include "initialization.h"
#include "router_globals.h"
#include "utils.h"

// static const char *TAG = "components/utils";

//-----------------------------------------------------------------------------
bool word_check(const char *str, const char *word)
{
    return (strstr(str, word) != NULL);
}

//-----------------------------------------------------------------------------
int bool_to_int(bool value)
{
    return value ? 1 : 0;
}

//-----------------------------------------------------------------------------
bool str_to_bool(const char *key)
{
    return strcmp(key, "true") == 0;
}

//-----------------------------------------------------------------------------
bool is_valid_ip(const char *ip_str)
{
    struct in_addr addr;
    int result = inet_pton(AF_INET, ip_str, &addr);
    return result != 0;
}

//-----------------------------------------------------------------------------
uint64_t system_uptime_usec(void)
{
    return esp_timer_get_time();
}

//-----------------------------------------------------------------------------
float system_uptime_s(void)
{
    return (float)(system_uptime_usec() / 1000000.0);
}

//-----------------------------------------------------------------------------
void restart(void)
{
    esp_restart();
}

//-----------------------------------------------------------------------------
char *param_set_default(const char *def_val)
{
    char *retval = malloc(strlen(def_val) + 1);
    strcpy(retval, def_val);
    return retval;
}


char* url_decode(const char* src) {
    if (src == NULL) {
        return NULL;
    }

    char* dest = malloc(strlen(src) + 1);
    if (dest == NULL) {
        return NULL;
    }

    char* p = dest;
    while (*src != '\0') {
        if (*src == '%' && *(src + 1) != '\0' && *(src + 2) != '\0') {
            char hex[3] = { *(src + 1), *(src + 2), '\0' };
            int i = strtol(hex, NULL, 16);
            *p++ = (char)i;
            src += 3;
        }
        else {
            *p++ = *src++;
        }
    }
    *p = '\0';
    return dest;
}

//-----------------------------------------------------------------------------
char *html_escape(const char *src)
{
    if (src == NULL)
    {
        return NULL;
    }
     // Decode the input string using url_decode
    char* decoded = url_decode(src);
     // Get the length of the decoded string
    int len = strlen(decoded);
     // Calculate the length of the escaped string
    int esc_len = len + 1;
    for (int i = 0; i < len; i++)
    {
        if (decoded[i] == '\\' || decoded[i] == '\'' || decoded[i] == '\"' || decoded[i] == '&' || decoded[i] == '#' || decoded[i] == ';')
        {
            esc_len += 5;
        }
    }
     // Allocate memory for the escaped string
    char *res = malloc(sizeof(char) * (esc_len + 1));
    if (res == NULL)
    {
        return NULL;
    }
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if (decoded[i] == '\\' || decoded[i] == '\'' || decoded[i] == '\"' || decoded[i] == '&' || decoded[i] == '#' || decoded[i] == ';')
        {
            res[j++] = '&';
            res[j++] = 'a';
            res[j++] = 'm';
            res[j++] = 'p';
            res[j++] = ';';
        }
        else
        {
            res[j++] = decoded[i];
        }
    }
    res[j] = '\0';
     // Free the memory allocated for the decoded string
    free(decoded);
     return res;
}