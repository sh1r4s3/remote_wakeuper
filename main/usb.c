/*
 * Copyright (c) 2024 Nikita Ermakov <sh1r4s3@mail.si-head.nl>
 * SPDX-License-Identifier: MIT
 */

#include "usb.h"

#include <esp_log.h>

#include <tinyusb.h>
#include <class/hid/hid_device.h>

#define KEYREPORT_DELAY pdMS_TO_TICKS(50)
#define WAKEUP_LOOP_DELAY pdMS_TO_TICKS(1000)

static const char * TAG = "RKW_USB";

// Total length of the configuration descriptor for TinyUSB
#define TUSB_DESC_TOTAL_LEN (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

// Descriptors for TinyUSB
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_ITF_PROTOCOL_KEYBOARD))
};

// USB HID configuration
const char *hid_string_descriptor[5] = {
    // array of pointer to string descriptors
    (char[]){0x09, 0x04},       // Is supported language is English (0x0409)
    "sh1r4s3",                 // Manufacturer
    "vkeyboard",               // Product
    "424242",                  // Serials, should use chip ID
    "Virtual ESP32 keyboard"   // HID
};

static const uint8_t hid_configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(1 /* configuration number */,
                          1 /* interface count */,
                          0 /* string index */,
                          TUSB_DESC_TOTAL_LEN /* total length */,
                          TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP /* attribute */,
                          100 /* mA */),

    TUD_HID_DESCRIPTOR(0 /* interface number */,
                       4 /* string index */,
                       false /* boot protocol */,
                       sizeof(hid_report_descriptor) /* report descriptor len */,
                       0x81 /* EP in address */,
                       16 /* size */,
                       10 /* polling interval */),
};

static void usb_callback() {
    ESP_LOGI(TAG, "USB callback");
}

void (*usb_get_callback(void))(void) {
    return usb_callback;
}

void usb_init() {
    ESP_LOGI(TAG, "Initializing USB");

    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG, "USB initialized");
}

int usb_send_data(uint8_t keycode) {
    ESP_LOGI(TAG, "Sending data");
    uint8_t keycode_arg[6] = {keycode};

    if (tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, keycode_arg))    return 1;
    vTaskDelay(KEYREPORT_DELAY);
    if (tud_hid_keyboard_report(HID_ITF_PROTOCOL_KEYBOARD, 0, NULL))    return 1;

    return 0;
}

void usb_hid_wakeup_loop() {
    while (1) {
        if (tud_mounted()) {
            tud_remote_wakeup();
        }
        vTaskDelay(WAKEUP_LOOP_DELAY);
    }
}

/* Invoked when received GET HID REPORT DESCRIPTOR request.
 * Application return pointer to descriptor, whose contents must exist long enough for transfer to complete. */
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    return hid_report_descriptor;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
}


