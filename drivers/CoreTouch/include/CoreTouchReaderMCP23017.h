// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_CORE_TOUCH_READER_MCP23017_H_
#define _LEKA_OS_CORE_TOUCH_READER_MCP23017_H_

#include "drivers/InterruptIn.h"

#include "CoreI2C.h"
#include "interface/drivers/TouchReader.h"

// ? MCP23017 driver datasheet available at:
// ? https://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf

namespace leka {

class CoreTouchReaderMCP23017 : public interface::TouchReader
{
  public:
	CoreTouchReaderMCP23017(interface::I2C &i2c, mbed::InterruptIn &irq_a, mbed::InterruptIn &irq_b)
		: _i2c(i2c), _irq_a(irq_a), _irq_b(irq_b) {};

	void initialize() final;
	auto readSensorsValues() -> uint8_t final;

	void registerOnTouchCallback(mbed::Callback<void()> on_touch_callback) final;

  private:
	interface::I2C &_i2c;
	mbed::InterruptIn &_irq_a;
	mbed::InterruptIn &_irq_b;

	const uint8_t _I2C_ADDRESS = 0x4E;

	// mbed::Callback<void()> _on_touch_callback;

	void setBankOrganization();
	void setIODirection();
	void setIOPolarity();
	void setIOPullUp();
	void setInterrupt();

	auto readRegister(uint8_t register_address) -> uint8_t;
	void writeRegister(uint8_t register_address, uint8_t value);
};

}	// namespace leka

#endif	 // _LEKA_OS_CORE_TOUCH_READER_MCP23017_H_
