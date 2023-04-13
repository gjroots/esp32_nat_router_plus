idf_component_register(SRCS esp32_nat_router.c
                    INCLUDE_DIRS include)

                    

target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/index.html.gz" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/settings.html.gz" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/info.html.gz" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/error_404.html.gz" TEXT)

target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/main.css.gz" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/dark.css.gz" TEXT)

target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/scan.js.gz" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/settings.js.gz" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/info.js.gz" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "../components/web_server/www/output/gzip/functions.js.gz" TEXT)