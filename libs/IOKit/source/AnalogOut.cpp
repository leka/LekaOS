// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IOKit/AnalogOut.h"
#include <array>

#include "MemoryUtils.h"

using namespace leka::io::expanded;

void AnalogOut::write(uint16_t value)
{
	auto buffer	 = std::array<uint8_t, 2> {};
	buffer.at(0) = utils::memory::getHighByte(value);
	buffer.at(1) = utils::memory::getLowByte(value);
	_dac.writeToSpecificInputRegister(_channel, buffer);
}
