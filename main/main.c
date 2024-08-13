/*
 * Copyright (c) 2024 Nikita Ermakov <sh1r4s3@mail.si-head.nl>
 * SPDX-License-Identifier: MIT
 */

#include <esp_log.h>
#include "wifi.h"
#include "usb.h"
#include "http.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char * TAG = "RKW_MAIN";

void app_main() {
    ESP_LOGI(TAG, "remote_wakeuper has started");
    wifi_init();
    usb_init();
    http_init(usb_get_callback());
    // Loop forever
    vTaskSuspend(NULL);
}
