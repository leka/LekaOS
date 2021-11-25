// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_FLAGS_H_
#define _LEKA_OS_FLAGS_H_

#include <cstdint>

constexpr uint32_t NEW_BLE_MESSAGE_FLAG(1UL << 1);
constexpr uint32_t BLE_PING_FLAG(1UL << 9);
constexpr uint32_t BLE_REBOOT_INSTRUCTION_FLAG(1UL << 8);
constexpr uint32_t BLE_LEDS_INTENSITY_FLAG(1UL << 2);
constexpr uint32_t BLE_LCD_INTENSITY_FLAG(1UL << 3);

constexpr uint32_t START_VIDEO_FLAG(1UL << 4);
constexpr uint32_t STOP_VIDEO_FLAG(1UL << 5);
constexpr uint32_t END_OF_VIDEO_FLAG(1UL << 7);

constexpr uint32_t NEW_RFID_TAG_FLAG(1UL << 6);

#endif	 // _LEKA_OS_FLAGS_H_
