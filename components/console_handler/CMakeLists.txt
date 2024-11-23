idf_component_register(SRCS console_handler.c file_system.c
                            
                    INCLUDE_DIRS include
                    REQUIRES cmd_router cmd_nvs nvs_flash fatfs json vfs esp_driver_uart esp_driver_usb_serial_jtag)