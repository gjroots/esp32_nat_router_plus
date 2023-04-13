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

    esp_err_t nvm_erase(void);
    esp_err_t nvm_set_bool(const char *key, bool value);
    void initialize_nvs(void);

#ifdef __cplusplus
}
#endif
