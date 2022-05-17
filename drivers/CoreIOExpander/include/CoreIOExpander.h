// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/DigitalOut.h"

#include "CoreI2C.h"
#include "external/MCP23017.h"
#include "interface/drivers/IOExpander.h"

namespace leka {

class CoreIOExpanderMCP23017 : public interface::IOExpander<uint16_t>
{
  public:
	explicit CoreIOExpanderMCP23017(interface::I2C &i2c, mbed::DigitalOut &mux_reset) : _i2c(i2c), _mux_reset(mux_reset)
	{
		init();
	};

	void init();

	void config_IODIR(uint16_t dir_config);

	void config_GPPU(uint16_t pullup_config);

	void config_IPOL(uint16_t polarity_config);

	void config_GPIO(uint16_t io_config);

	void setPinAsInput(uint16_t pin) final;

	auto readInputPin(uint16_t pin) -> int final;

	void setModeForInputPin(uint16_t pin, PinMode mode) final;

	auto getModeForInputPin(uint16_t pin) -> PinMode final;

	void setPinAsOutput(uint16_t pin) final;

	auto readOutputPin(uint16_t pin) -> int final;

	void writeOutputPin(uint16_t pin, int value) final;

  private:
	void writeRegister(uint8_t reg, uint16_t value);
	auto readRegister(uint8_t reg) -> uint16_t;

	interface::I2C &_i2c;
	mbed::DigitalOut &_mux_reset;
	const uint8_t _I2C_ADDRESS = 0x4E;
	uint16_t shadow_GPIO, shadow_IODIR, shadow_GPPU, shadow_IPOL;
};
}	// namespace leka
