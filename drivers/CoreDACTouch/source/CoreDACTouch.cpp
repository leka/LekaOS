// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX - License - Identifier : Apache - 2.0

#include "CoreDACTouch.h"
#include <array>

using namespace leka;

void CoreDACTouch::setVoltageReference(uint8_t voltageReference)
{
	uint8_t command = dac_touch::command::setVoltageReference | voltageReference;
	_i2c.write(_address, &command, 1, false);
}
