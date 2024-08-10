/*
 * Copyright (c) 2024 Nikita Ermakov <sh1r4s3@mail.si-head.nl>
 * SPDX-License-Identifier: MIT
 */

#ifndef RKW_USB
#define RKW_USB

#include "class/hid/hid.h"

void usb_init();
int usb_send_data(uint8_t keycode);
void (*usb_get_callback(void))(void);

#endif
