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

// ? MCP23017 driver datasheet available at:
// ? https://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf

namespace leka {

namespace mcp23017 {

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

class MCP23017
{
  public:
	enum Pin
	{
		Pin_PA0 = 0x0001,
		Pin_PA1 = 0x0002,
		Pin_PA2 = 0x0004,
		Pin_PA3 = 0x0008,
		Pin_PA4 = 0x0010,
		Pin_PA5 = 0x0020,
		Pin_PA6 = 0x0040,
		Pin_PA7 = 0x0080,

		Pin_PB0 = 0x0100,
		Pin_PB1 = 0x0200,
		Pin_PB2 = 0x0400,
		Pin_PB3 = 0x0800,
		Pin_PB4 = 0x1000,
		Pin_PB5 = 0x2000,
		Pin_PB6 = 0x4000,
		Pin_PB7 = 0x8000,
		Pin_All = 0xFFFF
	};

  protected:
	class ExpandedIO
	{
	  public:
		ExpandedIO(MCP23017 &parent, Pin pin) : _parent(parent), _pin(pin) {}

	  protected:
		auto internalRead() -> int;
		void internalMode(PinMode pull);
		void internalWrite(int value);
		void internalOutput();
		void internalInput();

	  private:
		MCP23017 &_parent;
		Pin _pin;
	};

  public:
	class ExpandedInput : public ExpandedIO, public mbed::interface::DigitalIn
	{
	  public:
		explicit ExpandedInput(MCP23017 &parent, Pin pin) : ExpandedIO(parent, pin) { internalInput(); }
		~ExpandedInput() override {}
		auto read() -> int override { return ExpandedIO::internalRead(); }
		void mode(PinMode pull) override { ExpandedIO::internalMode(pull); }
		auto is_connected() -> int override { return 1; }
	};

	/**
	 * ExpandedOutput class that implements the mbed::DigitalOut API
	 *
	 * @note See mbed::DigitalOut class declaration for API documentation
	 */
	// class ExpandedOutput : public ExpandedIO, public mbed::DigitalOut
	// {
	//   public:
	// 	ExpandedOutput(MCP23017 &parent, Pin pin) : ExpandedIO(parent, pin), mbed::DigitalOut(NC) { internal_output(); }
	// 	virtual ~ExpandedOutput() override {}
	// 	virtual void write(int value) override { ExpandedIO::internal_write(value); }
	// 	virtual int read() override { return ExpandedIO::internal_read(); }
	// 	virtual int is_connected() override { return 1; }
	// };

	/**
	 * ExpandedInputOutput class that implements the mbed::DigitalInOut API
	 *
	 * @note See mbed::DigitalInOut class declaration for API documentation
	 */
	// class ExpandedInputOutput : public ExpandedIO, public mbed::DigitalInOut
	// {
	//   public:
	// 	ExpandedInputOutput(MCP23017 &parent, Pin pin) : ExpandedIO(parent, pin), mbed::DigitalInOut(NC) { output(); }
	// 	virtual ~ExpandedInputOutput() override {}
	// 	virtual void write(int value) override { ExpandedIO::internal_write(value); }
	// 	virtual int read() override { return ExpandedIO::internal_read(); }
	// 	virtual void output() override { ExpandedIO::internal_output(); }
	// 	virtual void input() override { ExpandedIO::internal_input(); }
	// 	virtual void mode(PinMode pull) override { ExpandedIO::internal_mode(pull); }
	// 	virtual int is_connected() override { return 1; }
	// };

  public:
	/** Allow ExpandedInput/Output/InputOutput to access internal members*/
	friend class ChannelInput;
	friend class ChannelOutput;
	friend class ChannelInputOutput;

	explicit MCP23017(interface::I2C &i2c) : _i2c(i2c) {};

	void setRegisterMapping(bool separated = false);

	void init();

	auto asInput(Pin pin) -> ExpandedInput;

	// ExpandedOutput as_output(Pin pin);

	// ExpandedInputOutput as_input_output(Pin pin);

	void setInputPins(uint16_t pins);

	void setOutputPins(uint16_t pins);

	void writeOutputs(uint16_t values);

	auto readOutputs() -> uint16_t;

	uint16_t readInputs();

	void setInputPolarity(uint16_t values);

	auto getInputPolarity() -> uint16_t;

	void setPullups(uint16_t values);

	auto getPullups() -> uint16_t;

	void interruptOnChanges(uint16_t pins);

	void disableInterrupts(uint16_t pins);

	void acknowledgeInterrupt(uint16_t &pin, uint16_t &values);

	auto readRegister(uint8_t reg) -> uint16_t;

  private:
	void writeRegister(uint8_t reg, uint16_t value);

	void reset();

	interface::I2C &_i2c;
	const uint8_t _I2C_ADDRESS = 0x4E;

	PlatformMutex mutex;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_MCP23017_H_
