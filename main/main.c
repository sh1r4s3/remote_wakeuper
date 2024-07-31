/*
 * Copyright (c) 2024 Nikita Ermakov <sh1r4s3@mail.si-head.nl>
 * SPDX-License-Identifier: MIT
 */

#include <esp_log.h>
#include "wifi.h"
#include "usb.h"

static const char * TAG = "RKW_MAIN";

void app_main() {
    ESP_LOGI(TAG, "Virtual remote keyboard is welcoming you!");
    wifi_init();
    usb_init();
    usb_hid_wakeup_loop();
}
