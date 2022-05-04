// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/DigitalOut.h"

#include "CoreI2C.h"
#include "interface/drivers/IOExpander.h"
#include "internal/MCP23017.h"

namespace leka {

class CoreIOExpanderMCP23017 : public interface::IOExpander<uint16_t>
{
  public:
	explicit CoreIOExpanderMCP23017(interface::I2C &i2c);

	void init();

	void config(uint16_t dir_config, uint16_t pullup_config, uint16_t polarity_config);

	void writeOutputValue(uint16_t value);

	void setPinAsInput(uint16_t pin) final;

	void setPinAsOutput(uint16_t pin);

	auto readInputPin(uint16_t pin) -> int final;

	void setModeForPin(uint16_t pin, PinMode mode) final;

	auto getModeForPin(uint16_t pin) -> PinMode final;

  private:
	void writeRegister(uint8_t reg, uint16_t value);
	auto readRegister(uint8_t reg) -> uint16_t;

	interface::I2C &_i2c;
	const uint8_t _I2C_ADDRESS = 0x4E;
	uint16_t shadow_GPIO, shadow_IODIR, shadow_GPPU, shadow_IPOL;
	mbed::DigitalOut _mux_reset;
};
}	// namespace leka
