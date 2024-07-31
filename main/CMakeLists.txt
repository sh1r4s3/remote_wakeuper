idf_component_register(SRCS "main.c" "usb.c" "wifi.c" "http.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES esp_http_server esp_wifi nvs_flash)
