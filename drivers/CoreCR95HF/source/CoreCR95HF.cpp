// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"
#include <cstddef>
#include <cstdint>

#include "rtos/ThisThread.h"

using namespace std::chrono;

namespace leka {

void CoreCR95HF::send(const lstd::span<uint8_t> &command) {}

auto CoreCR95HF::receive(const lstd::span<uint8_t> &tag_anwser) -> size_t {}

auto CoreCR95HF::init() -> bool {}

}	// namespace leka
