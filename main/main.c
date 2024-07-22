/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"

#define SSID        CONFIG_ESP_WIFI_SSID
#define PWD         CONFIG_ESP_WIFI_PASSWORD
#define MAX_RETY    CONFIG_ESP_MAXIMUM_RETRY

static const char * TAG = "REMOTE_KEYBOARD";
static int nretry = 0;

void init_wifi() {
    // TODO
}

void app_main(void)
{
    ESP_LOGI(TAG, "Virtual remote keyboard is welcoming you!");
}
