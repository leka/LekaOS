// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/DACTouch.h"

namespace leka::io::expanded {

class AnalogOut
{
  public:
	explicit AnalogOut(interface::DACTouch &dac, uint8_t channel) : _dac(dac), _channel(channel) {};

	void write(uint16_t value);

  private:
	interface::DACTouch &_dac;
	uint8_t _channel;
};

}	// namespace leka::io::expanded
