idf_component_register(SRCS wifi_event_handler.c wifi_init.c wifi_handler.c
                            
                    INCLUDE_DIRS include
                    REQUIRES cmd_router cmd_nvs nvs_flash utils
                             esp_netif esp_wifi wpa_supplicant esp_hw_support router_handler)