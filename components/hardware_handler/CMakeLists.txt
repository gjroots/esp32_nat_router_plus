idf_component_register(SRCS hardware_handler.c led_handler.c 
                            button_handler.c
                    INCLUDE_DIRS include
                    REQUIRES cmd_router utils web_server)