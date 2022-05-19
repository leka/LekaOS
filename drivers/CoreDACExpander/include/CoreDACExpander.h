// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/drivers/DigitalOut.h>

#include "external/MCP4728.h"
#include "interface/drivers/I2C.h"

namespace leka {

class CoreDACExpanderMCP4728
{
  public:
	explicit CoreDACExpanderMCP4728(interface::I2C &i2c) : _i2c(i2c) {};

	void reset(uint8_t address, uint8_t channel);

	void multiple_write_for_dac_input_registers(uint8_t address, uint8_t channel, uint16_t value_calib);

	void single_write_for_dac_input_register_and_eeprom(uint8_t address, uint8_t channel, uint16_t value_calib);

	void set_vref(uint8_t address, uint8_t vref);

	void set_power_down(uint8_t address, uint8_t power_down);

	void set_gain(uint8_t address, uint8_t gain);

	static const uint8_t _I2C_ADDRESS_LEFT	= 0xC0;
	static const uint8_t _I2C_ADDRESS_RIGHT = 0xC2;

  private:
	interface::I2C &_i2c;
};
}	// namespace leka
