// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX - License - Identifier : Apache - 2.0

#ifndef _LEKA_OS_CORE_DAC_TOUCH_H_
#define _LEKA_OS_CORE_DAC_TOUCH_H_

#include "CoreI2C.h"
#include "interface/drivers/CoreDACTouch.h"

namespace leka {

namespace dac_touch {

	namespace command {

		constexpr uint8_t setVoltageReference			   = 0x80;
		constexpr uint8_t setPowerMode					   = 0xA0;
		constexpr uint8_t setGain						   = 0xC0;
		constexpr uint8_t writeAllInputRegister			   = 0x00;
		constexpr uint8_t writeSpecificInputRegister	   = 0x40;
		constexpr uint8_t writeMemoryRegisterUntilChannelD = 0x50;
		constexpr uint8_t writeSpecificMemoryRegister	   = 0x60;

	}	// namespace command

	namespace channel {

		constexpr uint8_t A = 0x00;
		constexpr uint8_t B = 0x01;
		constexpr uint8_t C = 0x02;
		constexpr uint8_t D = 0x03;

	}	// namespace channel

	namespace data {

		namespace voltage_reference {

			constexpr uint8_t Vdd		  = 0x00;
			constexpr uint8_t all_channel = 0x0f;
			constexpr uint8_t channel_A	  = 0x08;
			constexpr uint8_t channel_B	  = 0x04;
			constexpr uint8_t channel_C	  = 0x02;
			constexpr uint8_t channel_D	  = 0x01;

		}	// namespace voltage_reference

		namespace power_mode {
			constexpr uint8_t normal = 0x00;
			namespace channel_a {
				constexpr uint8_t normal		= 0x00;
				constexpr uint8_t powerDown1K	= 0x40;
				constexpr uint8_t powerDown100K = 0x80;
				constexpr uint8_t powerDown500K = 0xC0;
			}	// namespace channel_a
			namespace channel_b {
				constexpr uint8_t normal		= 0x00;
				constexpr uint8_t powerDown1K	= 0x10;
				constexpr uint8_t powerDown100K = 0x20;
				constexpr uint8_t powerDown500K = 0x30;
			}	// namespace channel_b
			namespace channel_c {
				constexpr uint8_t normal		= 0x00;
				constexpr uint8_t powerDown1K	= 0x04;
				constexpr uint8_t powerDown100K = 0x08;
				constexpr uint8_t powerDown500K = 0x0C;
			}	// namespace channel_c
			namespace channel_d {
				constexpr uint8_t normal		= 0x00;
				constexpr uint8_t powerDown1K	= 0x01;
				constexpr uint8_t powerDown100K = 0x02;
				constexpr uint8_t powerDown500K = 0x03;
			}	// namespace channel_d
		}		// namespace power_mode

		constexpr uint8_t gain_of_1_all = 0;

		namespace gain_of_2 {

			constexpr uint8_t channel_A = 0x08;
			constexpr uint8_t channel_B = 0x04;
			constexpr uint8_t channel_C = 0x02;
			constexpr uint8_t channel_D = 0x01;
			constexpr uint8_t all		= 0x0f;

		}	// namespace gain_of_2

	}	// namespace data

}	// namespace dac_touch

class CoreDACTouch : public interface::CoreDACTouch
{
  public:
	CoreDACTouch(interface::I2C &i2c, uint8_t address) : _i2c(i2c), _address(address) {};
	void setVoltageReference(uint8_t voltageReference) final;
	void setPowerMode(uint8_t powerMode) final;
	void setGain(uint8_t gain) final;

	void writeToAllInputRegister(std::array<uint8_t, 2> value) final;
	void writeToSpecificInputRegister(uint8_t channel, std::array<uint8_t, 2> value) final;

	void writeToMemoryRegisterUntilChannelD(uint8_t starting_channel, std::array<uint8_t, 2> value) final;
	void writeToSpecificMemoryRegister(uint8_t channel, std::array<uint8_t, 2> value) final;

	void readMemory(std::array<uint8_t, 24> &value) final;

  private:
	interface::I2C &_i2c;
	uint8_t _address;
};

}	// namespace leka

#endif	 // _LEKA_OS_CORE_DAC_TOUCH_H_
