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

	void write(uint8_t channel, uint16_t data) final;
	auto read(uint8_t channel) -> uint16_t final;

  private:
	void generalCallSoftwareUpdate();

	void writeInputRegisters();
	void readInputRegisters();

	void writeEEPROMRegisters(uint8_t channel);

	void setVoltageReference(uint8_t data);
	void setPowerDown(uint8_t data);
	void setGain(uint8_t data);

	interface::I2C &_i2c;
	uint8_t _address;

	struct QDACInputData {
		uint8_t vref  = 0x00;
		uint8_t pd	  = 0x00;
		uint8_t gain  = 0x00;
		uint16_t data = 0x0000;
	};

	std::array<QDACInputData, 4> _tx_registers {};
	std::array<QDACInputData, 4> _rx_registers {};

	static constexpr std::array<uint8_t, 4> _channels {mcp4728::channel::A, mcp4728::channel::B, mcp4728::channel::C,
													   mcp4728::channel::D};
};

}	// namespace leka
