// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <span>

#include "external/MCP4728.h"
#include "interface/drivers/I2C.h"
#include "interface/drivers/QDAC.h"

namespace leka {

class CoreQDACMCP4728 : public interface::QDAC
{
  public:
	CoreQDACMCP4728(interface::I2C &i2c, uint8_t address) : _i2c(i2c), _address(address) {};

	void init() final;
	void write(uint8_t channel, std::array<uint8_t, 2> data) final;
	void read(std::span<uint8_t> buffer) final;

  private:
	interface::I2C &_i2c;
	uint8_t _address;

	void writeToMultipleInputRegisters(uint8_t channel, std::array<uint8_t, 2> data);
	void writeToInputRegistersAndMemoryUntilChannelD(uint8_t channel, std::array<uint8_t, 2> data);
	void writeToSingleInputRegisterAndMemory(uint8_t channel, std::array<uint8_t, 2> data);

	void writeVoltageReference(uint8_t voltageReference);
	void writePowerMode(uint8_t powerMode);
	void writeGain(uint8_t gain);

	void readInputRegistersAndMemory(std::span<uint8_t> buffer);
};

}	// namespace leka
