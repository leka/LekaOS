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
		constexpr uint8_t setPowerMode		  = 0xA0;
		constexpr uint8_t setGain			  = 0xC0;
		constexpr uint8_t writeInputRegister  = 0x00;

	}	// namespace command

	namespace data {

		namespace voltageReference {

			constexpr uint8_t Vdd		  = 0x00;
			constexpr uint8_t all_channel = 0x0f;
			constexpr uint8_t channel_A	  = 0x08;
			constexpr uint8_t channel_B	  = 0x04;
			constexpr uint8_t channel_C	  = 0x02;
			constexpr uint8_t channel_D	  = 0x01;

		}	// namespace voltageReference

		namespace powerMode {
			constexpr uint8_t normal = 0x00;
			namespace channelA {
				constexpr uint8_t normal		= 0x00;
				constexpr uint8_t powerDown1K	= 0x40;
				constexpr uint8_t powerDown100K = 0x80;
				constexpr uint8_t powerDown500K = 0xC0;
			}	// namespace channelA
			namespace channelB {
				constexpr uint8_t normal		= 0x00;
				constexpr uint8_t powerDown1K	= 0x10;
				constexpr uint8_t powerDown100K = 0x20;
				constexpr uint8_t powerDown500K = 0x30;
			}	// namespace channelB
			namespace channelC {
				constexpr uint8_t normal		= 0x00;
				constexpr uint8_t powerDown1K	= 0x04;
				constexpr uint8_t powerDown100K = 0x08;
				constexpr uint8_t powerDown500K = 0x0C;
			}	// namespace channelC
			namespace channelD {
				constexpr uint8_t normal		= 0x00;
				constexpr uint8_t powerDown1K	= 0x01;
				constexpr uint8_t powerDown100K = 0x02;
				constexpr uint8_t powerDown500K = 0x03;
			}	// namespace channelD
		}		// namespace powerMode

		namespace gain2 {

			constexpr uint8_t channel_A = 0x08;
			constexpr uint8_t channel_B = 0x04;
			constexpr uint8_t channel_C = 0x02;
			constexpr uint8_t channel_D = 0x01;
			constexpr uint8_t all		= 0x0f;

		}	// namespace gain2

	}	// namespace data

}	// namespace dac_touch

class CoreDACTouch
{
  public:
	CoreDACTouch(interface::I2C &i2c, uint8_t address) : _i2c(i2c), _address(address) {};
	void setVoltageReference(uint8_t voltageReference);
	void setPowerMode(uint8_t powerMode);
	void setGain(uint8_t gain);

	void writeToInputRegister(std::array<uint8_t, 2> value);

  private:
	interface::I2C &_i2c;
	uint8_t _address;
};

}	// namespace leka

#endif	 // _LEKA_OS_CORE_DAC_TOUCH_H_
