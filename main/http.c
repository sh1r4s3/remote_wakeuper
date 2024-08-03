/*
 * Copyright (c) 2024 Nikita Ermakov <sh1r4s3@mail.si-head.nl>
 * SPDX-License-Identifier: MIT
 */

#include <esp_log.h>
#include <esp_http_server.h>
#include <stdlib.h>

static const char * TAG = "RKW_HTTP";

static esp_err_t uri_root_handler(httpd_req_t * request);

static httpd_handle_t g_server_handle = NULL;
static httpd_uri_t g_get_uris[] = {
    {
        .uri      = "/",
        .method   = HTTP_GET,
        .handler  = uri_root_handler,
        .user_ctx = NULL
    }
};

static void (*usb_callback)(void) = NULL;

void http_init(void (*pfn)(void)) {
    ESP_LOGI(TAG, "Starting HTTP server");
    // Get the default HTTP server configuration
    httpd_config_t http_config = HTTPD_DEFAULT_CONFIG();

    // Register a USB callback
    if (pfn) {
        usb_callback = pfn;
    } else {
        ESP_LOGE(TAG, "Pointer to a callback function is NULL");
        abort();
    }

    // Start HTTP server
    ESP_ERROR_CHECK(httpd_start(&g_server_handle, &http_config));
    ESP_LOGI(TAG, "HTTP server has been started. Listening %d", http_config.server_port);

    for (int i = 0; i < sizeof(g_get_uris)/sizeof(g_get_uris[0]); ++i) {
        ESP_LOGI(TAG, "Registering GET handler for %s", g_get_uris[i].uri);
        ESP_ERROR_CHECK(httpd_register_uri_handler(g_server_handle, g_get_uris + i));
    }
}

esp_err_t uri_root_handler(httpd_req_t * request) {
    const char * response = "Processed";
    ESP_LOGI(TAG, "got a request for /");
    httpd_resp_send(request, response, HTTPD_RESP_USE_STRLEN);
    usb_callback();
    return ESP_OK;
}
