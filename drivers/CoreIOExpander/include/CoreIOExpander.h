// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0
// Original file:
// https://github.com/AGlass0fMilk/ep-oc-mcu/blob/refine-io-drivers/devices/MCP23008/MCP23008.hpp

#ifndef _LEKA_OS_DRIVER_MCP23017_H_
#define _LEKA_OS_DRIVER_MCP23017_H_

#include <cstdint>

#include "drivers/DigitalIn.h"
#include "drivers/interfaces/InterfaceDigitalIn.h"

#include "CoreI2C.h"
#include "interface/drivers/CoreIOExpander.h"

// * MCP23017 driver datasheet available at:
// * https://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf

namespace leka {

namespace mcp23017 {

	namespace pin {

		constexpr uint16_t PA0 = 0x0001;
		constexpr uint16_t PA1 = 0x0002;
		constexpr uint16_t PA2 = 0x0004;
		constexpr uint16_t PA3 = 0x0008;
		constexpr uint16_t PA4 = 0x0010;
		constexpr uint16_t PA5 = 0x0020;
		constexpr uint16_t PA6 = 0x0040;
		constexpr uint16_t PA7 = 0x0080;

		constexpr uint16_t PB0 = 0x0100;
		constexpr uint16_t PB1 = 0x0200;
		constexpr uint16_t PB2 = 0x0400;
		constexpr uint16_t PB3 = 0x0800;
		constexpr uint16_t PB4 = 0x1000;
		constexpr uint16_t PB5 = 0x2000;
		constexpr uint16_t PB6 = 0x4000;
		constexpr uint16_t PB7 = 0x8000;
		constexpr uint16_t All = 0xFFFF;

	}	// namespace pin

	namespace registers {

		constexpr uint8_t IODIR	  = 0x00;
		constexpr uint8_t IPOL	  = 0x02;
		constexpr uint8_t GPINTEN = 0x04;
		constexpr uint8_t DEFVAL  = 0x06;
		constexpr uint8_t INTCON  = 0x08;
		constexpr uint8_t IOCON	  = 0x0A;
		constexpr uint8_t GPPU	  = 0x0C;
		constexpr uint8_t INTF	  = 0x0E;
		constexpr uint8_t INTCAP  = 0x10;
		constexpr uint8_t GPIO	  = 0x12;
		constexpr uint8_t OLAT	  = 0x14;

	}	// namespace registers

	// namespace register

	namespace directory {

		constexpr uint8_t DIR_OUTPUT = 0x00;
		constexpr uint8_t DIR_INPUT	 = 0x01;

	}	// namespace directory

}	// namespace mcp23017

class MCP23017 : public interface::CoreIOExpander
{
  protected:
	class ExpandedIO
	{
	  public:
		ExpandedIO(interface::CoreIOExpander &parent, uint16_t pin) : _parent(parent), _pin(pin) {}

	  protected:
		auto internalRead() -> int;
		void internalMode(PinMode mode);
		void internalInput();

	  private:
		interface::CoreIOExpander &_parent;
		uint16_t _pin;
	};

  public:
	class ExpandedInput : public ExpandedIO, public mbed::interface::DigitalIn
	{
	  public:
		explicit ExpandedInput(interface::CoreIOExpander &parent, uint16_t pin) : ExpandedIO(parent, pin)
		{
			internalInput();
		}
		~ExpandedInput() override = default;
		auto read() -> int override { return ExpandedIO::internalRead(); }
		void mode(PinMode mode) override { ExpandedIO::internalMode(mode); }
		auto is_connected() -> int override { return 1; }
	};

	explicit MCP23017(interface::I2C &i2c) : _i2c(i2c) {};

	void setRegisterMapping(bool separated = false) final;

	void init(uint16_t input_pins) final;

	auto asInput(uint16_t pin) -> ExpandedInput;

	void setInputPins(uint16_t pins) final;
	void setOutputPins(uint16_t pins) final;

	void writeOutputs(uint16_t values) final;

	auto readOutputs() -> uint16_t final;
	auto readInputs() -> uint16_t final;

	void setInputPolarity(uint16_t values) final;
	auto getInputPolarity() -> uint16_t final;

	void setPullups(uint16_t values) final;
	auto getPullups() -> uint16_t final;

	void interruptOnChanges(uint16_t pins) final;
	void disableInterrupts(uint16_t pins) final;

	void acknowledgeInterrupt(uint16_t &pin, uint16_t &values) final;

  private:
	void writeRegister(uint8_t reg, uint16_t value);
	auto readRegister(uint8_t reg) -> uint16_t;

	void reset();

	interface::I2C &_i2c;
	const uint8_t _I2C_ADDRESS = 0x4E;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_MCP23017_H_
