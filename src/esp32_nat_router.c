#include <stdio.h>
#include <esp_system.h>
#include <esp_log.h>
#include <lwip/lwip_napt.h>

#include "cmd_decl.h"
#include "router_globals.h"
#include "get_data_handler.h"
#include "auth_handler.h"
#include "initialization.h"
#include "hardware_handler.h"
#include "web_server.h"
#include "console_handler.h"
#include "file_system.h"
#include "mac_generator.h"
#include "nvm.h"
#include "router_handler.h"
#include "wifi_init.h"
#include "ota_handler.h"

#if !IP_NAPT
#error "IP_NAPT must be defined"
#endif

/* Global vars */
uint16_t connect_count = 0;
bool ap_connect = false;

uint32_t my_ip;
uint32_t my_ap_ip;

static const char *TAG = "ESP32 NAT router +";


//-----------------------------------------------------------------------------
void app_main(void)
{
    initialize_nvs();

#if CONFIG_STORE_HISTORY
    initialize_filesystem();
    ESP_LOGI(TAG, "Command history enabled");
#else
    ESP_LOGI(TAG, "Command history disabled");
#endif

    ESP_ERROR_CHECK(parms_init());
    hardware_init();
    get_portmap_tab();
    wifi_init();
    ip_napt_enable(my_ap_ip, 1);
    ESP_LOGI(TAG, "NAT is enabled");

    if (IsWebServerEnable)
    {
        ESP_LOGI(TAG, "Starting config web server");
        server = start_webserver();
    }

    ota_update_init();
    initialize_console();
    register_system();
    register_nvs();
    register_router();
    start_console();
}

//-----------------------------------------------------------------------------