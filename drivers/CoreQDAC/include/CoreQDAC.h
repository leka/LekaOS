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
	void write(uint8_t channel, uint16_t data, bool b_eep = false) final;
	auto read(uint8_t channel, bool b_eep = false) -> uint16_t final;

	void writeAllChannels(uint16_t data, bool b_eep = false);

  private:
	void fastWrite();
	void multiWrite();
	void sequentialWrite();
	void singleWrite(uint8_t channel);

	void selectVoltageReference(uint8_t data);
	void selectPowerMode(uint8_t data);
	void selectGain(uint8_t data);

	void readInputRegistersAndMemory();

	interface::I2C &_i2c;
	uint8_t _address;

	struct DACInputData {
		uint8_t vref  = 0x00;
		uint8_t pd	  = 0x00;
		uint8_t gain  = 0x00;
		uint16_t data = 0x0000;
	};

	std::array<DACInputData, 4> _reg {};
	std::array<DACInputData, 4> _eep {};
	std::array<DACInputData, 4> _read_reg {};
	std::array<DACInputData, 4> _read_eep {};
};

}	// namespace leka
