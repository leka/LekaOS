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

/** MCP23017 class
 *
 * Allow access to an I2C connected MCP23017 16-bit I/O extender chip
 *
 */

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
	// enum Frequency
	// {
	// 	Frequency_100KHz = 100000,
	// 	Frequency_400KHz = 400000,
	// 	/* Note: 1.7MHz probably won't work for mbed */
	// 	Frequency_1700KHz = 1700000
	// };
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
	/**
	 * Expanded IO class implements functionality common to
	 * ExpandedInput, ExpandedOutput, ExpandedInputOutput
	 *
	 * @note Accessing an ExpandedIO from multiple handles
	 * (ie: using it with both ExpandedInput and ExpandedOutput,
	 * or using the MCP23017 APIs directly) may cause unexpected
	 * behavior. The state of an ExpandedIO's direction
	 * is not maintained past initialization.
	 */
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

	  protected:
		MCP23017 &_parent;
		Pin _pin;
	};

  public:
	/**
	 * ExpandedInput class that implements the mbed::DigitalIn API
	 *
	 * @note See mbed::DigitalIn class declaration for API documentation
	 */
	class ExpandedInput : public ExpandedIO, public mbed::interface::DigitalIn
	{
	  public:
		ExpandedInput(MCP23017 &parent, Pin pin) : ExpandedIO(parent, pin) { internalInput(); }
		virtual ~ExpandedInput() override {}
		virtual auto read() -> int override { return ExpandedIO::internalRead(); }
		virtual void mode(PinMode pull) override { ExpandedIO::internalMode(pull); }
		virtual auto is_connected() -> int override { return 1; }
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

	/** Constructor
	 *
	 * @param sda I2C sda pin
	 * @param scl I2C scl pin
	 * @param address The hardware address of the MCP23017. This is the 3-bit
	 * value that is physically set via A0, A1, and A2.
	 * @param freq The I2C frequency. Should probably be 100KHz or 400KHz.
	 */

	MCP23017(interface::I2C &i2c) : _i2c(i2c) {};

	/** Set the registers associated with each port as being separated into different banks or in the same bank
	 *
	 * This function sets how the registers are addressed for ports A and B.
	 * If the bit is set to 1, ports are segregated: registers of port A are in a row followed by registers of port B.
	 * If the bit is set to 0, ports are paired: each register of port A is followed by the same of port B.
	 *
	 * @param separated True if registers of each port are separated.
	 */
	void setRegisterMapping(bool separated = false);

	void init();

	/**
	 * Convenience function to create a DigitalIn object for a given pin
	 * @param[in] pin Pin to instantiate as DigitalIn
	 * @retval out DigitalIn-like object
	 *
	 * @note you can cast a pointer to the returned object to a DigitalIn*
	 * to pass it to APIs that require a DigitalIn* object
	 */
	auto asInput(Pin pin) -> ExpandedInput;

	/**
	 * Convenience function to create a DigitalOut object for a given pin
	 * @param[in] pin Pin to instantiate as DigitalOut
	 * @retval out DigitalOut-like object
	 */
	// ExpandedOutput as_output(Pin pin);

	/**
	 * Convenience function to create a DigitalInOut object for a given pin
	 * @param[in] pin Pin to instantiate as DigitalInOut
	 * @retval inout DigitalInOut-like object
	 */
	// ExpandedInputOutput as_input_output(Pin pin);

	/** Set pins to input mode
	 *
	 * This function is used to set which pins are inputs (if any). Example:
	 * set_inputs ( Pin_GP0 | Pin_GP1 | Pin_GP2 );
	 * Note that these are set to input in addition to the previously set.
	 * In other words, the following:
	 * set_inputs ( Pin_GP1 );
	 * set_inputs ( Pin_GP2 );
	 * Results in at least two pins set to input.
	 *
	 * @param pins A bitmask of pins to set to input mode.
	 */
	void setInputPins(uint8_t pins);
	/** Set pins to output mode
	 *
	 * This function is used to set which pins are outputs (if any). Example:
	 * set_outputs ( Pin_GP0 | Pin_GP1 | Pin_GP2 );
	 * Note that these are set to output in addition to the previously set.
	 * In other words, the following:
	 * set_outputs ( Pin_GP1 );
	 * set_outputs ( Pin_GP2 );
	 * Results in at least two pins set to output.
	 *
	 * @param pins A bitmask of pins to set to output mode.
	 */
	void setOutputPins(uint8_t pins);

	/** Write to the output pins.
	 *
	 * This function is used to set output pins on or off.
	 *
	 * @param values A bitmask indicating whether a pin should be on or off.
	 */
	void writeOutputs(uint16_t values);
	/** Read back the outputs.
	 *
	 * This function is used to read the last values written to the output pins.
	 *
	 * @returns The value from the OLAT register.
	 */
	auto readOutputs() -> uint16_t;

	/** Read from the input pins.
	 *
	 * This function is used to read the values from the input pins.
	 *
	 * @returns A bitmask of the current state of the input pins.
	 */
	uint16_t readInputs();

	/** Set the input pin polarity.
	 *
	 * This function sets the polarity of the input pins.
	 * A 1 bit is inverted polarity, a 0 is normal.
	 *
	 * @param values A bitmask of the input polarity.
	 */
	void setInputPolarity(uint16_t values);
	/** Read back the current input pin polarity.
	 *
	 * This function reads the current state of the input pin polarity.
	 *
	 * @returns The value from the IPOL register.
	 */
	auto getInputPolarity() -> uint16_t;

	/** Enable and disable the internal pull-up resistors for input pins.
	 *
	 * This function enables the internal 100 kΩ pull-up resistors.
	 * A 1 bit enables the pull-up resistor for the corresponding input pin.
	 *
	 * @param values A bitmask indicating which pull-up resistors should be enabled/disabled.
	 */
	void setPullups(uint16_t values);
	/** Get the current state of the internal pull-up resistors.
	 *
	 * @returns The current state of the pull-up resistors.
	 */
	auto getPullups() -> uint16_t;

	/** Generate an interrupt when a pin changes.
	 *
	 * This function enables interrupt generation for the specified pins.
	 * The interrupt is active-low by default.
	 * The function acknowledge_interrupt must be called before another
	 * interrupt will be generated.
	 * Example:
	 * @code
	 * InterruptIn in ( p16 );
	 * MCP23017 mcp ( p9, p10, 0 );
	 * in.fall ( &interrupt );
	 * mcp.interrupt_on_changes ( MCP23017::Pin_GP0 );
	 * while ( 1 ) {
	 *      wait ( 1 );
	 * }
	 * @endcode
	 *
	 * @param pins A bitmask of the pins that may generate an interrupt.
	 */
	void interruptOnChanges(uint16_t pins);
	/** Disables interrupts for the specified pins.
	 *
	 * @param values A bitmask indicating which interrupts should be disabled.
	 */
	void disableInterrupts(uint16_t pins);

	/** Acknowledge a generated interrupt.
	 *
	 * This function must be called when an interrupt is generated to discover
	 * which pin caused the interrupt and to enable future interrupts.
	 *
	 * @param pin An output paramter that specifies which pin generated the interrupt.
	 * @param values The current state of the input pins.
	 */
	void acknowledgeInterrupt(uint16_t &pin, uint16_t &values);

  protected:
	auto readRegister(uint8_t reg) -> uint16_t;
	void writeRegister(uint8_t reg, uint16_t value);

	void reset();

	interface::I2C &_i2c;
	const uint8_t _I2C_ADDRESS = 0x4E;

	PlatformMutex mutex;

  private:
	unsigned short shadow_GPIO, shadow_IODIR, shadow_GPPU, shadow_IPOL,
		shadow_GPINTEN;	  // Cached copies of the register values
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_MCP23017_H_
