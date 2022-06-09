// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cstdint"

namespace leka::mcp4728 {

namespace command {

	inline constexpr uint8_t writeAllInputRegister			  = 0x00;
	inline constexpr uint8_t writeSpecificInputRegister		  = 0x40;
	inline constexpr uint8_t writeMemoryRegisterUntilChannelD = 0x50;
	inline constexpr uint8_t writeSpecificMemoryRegister	  = 0x60;
	inline constexpr uint8_t writeVoltageReference			  = 0x80;
	inline constexpr uint8_t writePowerMode					  = 0xA0;
	inline constexpr uint8_t writeGain						  = 0xC0;

}	// namespace command

namespace channel {

	inline constexpr uint8_t A = 0x00;
	inline constexpr uint8_t B = 0x01;
	inline constexpr uint8_t C = 0x02;
	inline constexpr uint8_t D = 0x03;

}	// namespace channel

namespace data {

	namespace voltage_reference {

		inline constexpr uint8_t Vdd = 0x00;
		namespace internal {
			inline constexpr uint8_t channel_A	 = 0x08;
			inline constexpr uint8_t channel_B	 = 0x04;
			inline constexpr uint8_t channel_C	 = 0x02;
			inline constexpr uint8_t channel_D	 = 0x01;
			inline constexpr uint8_t all_channel = 0x0f;

		}	// namespace internal

	}	// namespace voltage_reference

	namespace power_mode {

		inline constexpr uint8_t normal = 0x00;

		namespace channel_a {
			inline constexpr uint8_t normal		   = 0x00;
			inline constexpr uint8_t powerDown1K   = 0x40;
			inline constexpr uint8_t powerDown100K = 0x80;
			inline constexpr uint8_t powerDown500K = 0xC0;
		}	// namespace channel_a

		namespace channel_b {
			inline constexpr uint8_t normal		   = 0x00;
			inline constexpr uint8_t powerDown1K   = 0x10;
			inline constexpr uint8_t powerDown100K = 0x20;
			inline constexpr uint8_t powerDown500K = 0x30;
		}	// namespace channel_b

		namespace channel_c {
			inline constexpr uint8_t normal		   = 0x00;
			inline constexpr uint8_t powerDown1K   = 0x04;
			inline constexpr uint8_t powerDown100K = 0x08;
			inline constexpr uint8_t powerDown500K = 0x0C;
		}	// namespace channel_c

		namespace channel_d {
			inline constexpr uint8_t normal		   = 0x00;
			inline constexpr uint8_t powerDown1K   = 0x01;
			inline constexpr uint8_t powerDown100K = 0x02;
			inline constexpr uint8_t powerDown500K = 0x03;
		}	// namespace channel_d

	}	// namespace power_mode

	namespace gain {

		namespace x1 {
			inline constexpr uint8_t all = 0x00;
		}

		namespace x2 {
			inline constexpr uint8_t channel_A = 0x08;
			inline constexpr uint8_t channel_B = 0x04;
			inline constexpr uint8_t channel_C = 0x02;
			inline constexpr uint8_t channel_D = 0x01;
			inline constexpr uint8_t all	   = 0x0f;
		}	// namespace x2

	}	// namespace gain

}	// namespace data

}	// namespace leka::mcp4728
