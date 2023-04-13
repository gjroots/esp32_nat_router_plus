idf_component_register(SRCS request_handler.c response_handler.c get_data_handler.c
                            storage_handler.c auth_handler.c web_server.c
                    
                    INCLUDE_DIRS include 
                    REQUIRES esp_http_server esp-tls json esp_http_client utils 
                                cmd_router cmd_nvs ota_handler wifi_handler)



