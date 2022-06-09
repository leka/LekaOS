// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>

#include "external/MCP4728.h"
#include "interface/drivers/DACTouch.h"
#include "interface/drivers/I2C.h"

namespace leka {

class CoreDACTouchMCP4728 : public interface::DACTouch
{
  public:
	CoreDACTouchMCP4728(interface::I2C &i2c, uint8_t address) : _i2c(i2c), _address(address) {};

	void writeToAllInputRegister(std::array<uint8_t, 2> value) final;
	void writeToSpecificInputRegister(uint8_t channel, std::array<uint8_t, 2> value) final;
	void writeToMemoryRegisterUntilChannelD(uint8_t starting_channel, std::array<uint8_t, 2> value) final;
	void writeToSpecificMemoryRegister(uint8_t channel, std::array<uint8_t, 2> value) final;

	void writeVoltageReference(uint8_t voltageReference) final;
	void writePowerMode(uint8_t powerMode) final;
	void writeGain(uint8_t gain) final;

	void readMemory(std::array<uint8_t, 24> &value) final;

  private:
	interface::I2C &_i2c;
	uint8_t _address;
};

}	// namespace leka
