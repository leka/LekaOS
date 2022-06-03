// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "external/MCP23017.h"
#include "interface/drivers/DigitalOut.h"
#include "interface/drivers/I2C.h"
#include "interface/drivers/IOExpander.h"

namespace leka {

class CoreIOExpanderMCP23017 : public interface::IOExpander<uint16_t>
{
  public:
	explicit CoreIOExpanderMCP23017(interface::I2C &i2c, interface::DigitalOut &mux_reset);

	void setPinAsInput(uint16_t pin) final;

	auto readInputPin(uint16_t pin) -> int final;

	void setModeForPin(uint16_t pin, PinMode mode) final;

	auto getModeForPin(uint16_t pin) -> PinMode final;

	void setPinAsOutput(uint16_t pin) final;

	auto readOutputPin(uint16_t pin) -> int final;

	void writeOutputPin(uint16_t pin, int value) final;

  private:
	void init();
	void writeRegister(uint8_t reg, uint16_t value);
	auto readRegister(uint8_t reg) -> uint16_t;

	interface::I2C &_i2c;
	interface::DigitalOut &_mux_reset;
	const uint8_t _I2C_ADDRESS = 0x4E;

	struct {
		uint16_t iodir	 = 0xFFFF;
		uint16_t ipol	 = 0x0000;
		uint16_t gpinten = 0x0000;
		uint16_t defval	 = 0x0000;
		uint16_t intcon	 = 0x0000;
		uint16_t iocon	 = 0x0000;
		uint16_t gppu	 = 0x0000;
		uint16_t intf	 = 0x0000;
		uint16_t intcap	 = 0x0000;
		uint16_t gpio	 = 0x0000;
		uint16_t olat	 = 0x0000;
	} _shadow_register;
};
}	// namespace leka
