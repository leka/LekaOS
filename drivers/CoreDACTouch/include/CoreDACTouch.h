// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX - License - Identifier : Apache - 2.0

#ifndef _LEKA_OS_CORE_DAC_TOUCH_H_
#define _LEKA_OS_CORE_DAC_TOUCH_H_

#include "CoreI2C.h"

namespace leka {

namespace dac_touch {

	namespace command {

		constexpr uint8_t setVoltageReference = 0x80;
		constexpr uint8_t powerMode			  = 0xA0;

	}	// namespace command

	namespace data {

		namespace voltageReference {

			constexpr uint8_t Vdd		  = 0x00;
			constexpr uint8_t all_channel = 0x0f;
			constexpr uint8_t channel_A	  = 0x01;
			constexpr uint8_t channel_B	  = 0x02;
			constexpr uint8_t channel_C	  = 0x04;
			constexpr uint8_t channel_D	  = 0x08;

		}	// namespace voltageReference

	}	// namespace data

}	// namespace dac_touch

class CoreDACTouch
{
  public:
	CoreDACTouch(interface::I2C &i2c, uint8_t address) : _i2c(i2c), _address(address) {};
	void setVoltageReference(uint8_t voltageReference);

  private:
	interface::I2C &_i2c;
	uint8_t _address;
};

}	// namespace leka

#endif	 // _LEKA_OS_CORE_DAC_TOUCH_H_
