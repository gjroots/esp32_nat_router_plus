idf_component_register(SRCS "cmd_router.c"
                    INCLUDE_DIRS .
                    REQUIRES console esp_wifi driver nvs_flash)
