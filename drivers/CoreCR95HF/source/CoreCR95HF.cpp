
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

namespace leka {

CoreCR95HF::CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {}

void CoreCR95HF::send(uint8_t *data, size_t size) {}

void CoreCR95HF::receive(uint8_t *data, size_t size) {}

}	// namespace leka
