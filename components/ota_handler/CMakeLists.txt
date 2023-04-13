idf_component_register(SRCS "ota_handler.c" 
                    INCLUDE_DIRS include
                    REQUIRES esp_http_server app_update cmd_router)



