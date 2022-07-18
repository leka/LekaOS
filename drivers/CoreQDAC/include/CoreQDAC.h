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
	void write(Channel channel, uint16_t data, bool eeprom = false) final;
	void writeAllChannels(uint16_t data, bool eeprom = false);

	auto read(Channel channel, bool eeprom = false) -> uint16_t final;

  private:
	void fastWrite();
	void multiWrite(uint8_t channel);
	void sequentialWrite(uint8_t starting_channel);
	void singleWrite(uint8_t channel);

	void setVoltageReference(uint8_t data);
	void setPowerDown(uint8_t data);
	void setGain(uint8_t data);

	void readInputRegistersAndMemory();

	interface::I2C &_i2c;
	uint8_t _address;

	struct QDACInputData {
		uint8_t vref  = 0x00;
		uint8_t pd	  = 0x00;
		uint8_t gain  = 0x00;
		uint16_t data = 0x0000;
	};

	std::array<QDACInputData, 4> _tx_registers {};
	std::array<QDACInputData, 4> _tx_eeprom {};
	std::array<QDACInputData, 4> _rx_registers {};
	std::array<QDACInputData, 4> _rx_eeprom {};
};

}	// namespace leka
