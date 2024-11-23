idf_component_register(SRCS "cmd_system.c"
                    INCLUDE_DIRS .
                    REQUIRES console driver esp_wifi spi_flash esp_app_format)