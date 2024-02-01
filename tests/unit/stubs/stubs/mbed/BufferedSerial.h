// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

namespace leka {

extern std::vector<uint8_t> spy_BufferedSerial_read_values;
extern std::vector<uint8_t> spy_BufferedSerial_write_values;
extern mbed::Callback<void()> spy_BufferedSerial_sigio_callback;

void spy_BufferedSerial_setValuesBeforeRead(std::vector<uint8_t> values);
auto spy_BufferedSerial_getValuesAfterWrite() -> std::vector<uint8_t>;

auto spy_BufferedSerial_getSigioCallback() -> mbed::Callback<void()>;

auto spy_BufferedSerial_getEnableInput() -> bool;

}	// namespace leka
