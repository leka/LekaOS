// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_FLAGS_H_
#define _LEKA_OS_FLAGS_H_

#include <cstdint>

constexpr uint32_t NEW_BLE_MESSAGE_FLAG(1UL << 0);

constexpr uint32_t BLE_PING_FLAG(1UL << 1);
constexpr uint32_t BLE_REBOOT_INSTRUCTION_FLAG(1UL << 2);
constexpr uint32_t BLE_MODE_FLAG(1UL << 3);

constexpr uint32_t ENABLE_LEDS_FLAG(1UL << 4);
constexpr uint32_t ENABLE_SCREEN_FLAG(1UL << 5);
constexpr uint32_t ENABLE_RFID_FLAG(1UL << 6);
constexpr uint32_t ENABLE_MOTORS_FLAG(1UL << 7);

constexpr uint32_t START_VIDEO_FLAG(1UL << 10);
constexpr uint32_t STOP_VIDEO_FLAG(1UL << 11);
constexpr uint32_t END_OF_VIDEO_FLAG(1UL << 12);

constexpr uint32_t MODE_TURN_OFF_ALL_FLAG(1UL << 13);
constexpr uint32_t MODE_USER1_FLAG(ENABLE_LEDS_FLAG + ENABLE_SCREEN_FLAG + ENABLE_RFID_FLAG);
constexpr uint32_t MODE_USER2_FLAG(ENABLE_LEDS_FLAG + ENABLE_SCREEN_FLAG + ENABLE_RFID_FLAG + ENABLE_MOTORS_FLAG);

#endif	 // _LEKA_OS_FLAGS_H_
