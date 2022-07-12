// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "cstdint"

namespace leka::mcp4728 {

namespace command {

	inline constexpr auto fast_write	   = uint8_t {0x00};
	inline constexpr auto multi_write	   = uint8_t {0x40};
	inline constexpr auto sequential_write = uint8_t {0x50};
	inline constexpr auto single_write	   = uint8_t {0x58};

	inline constexpr auto select_vref		= uint8_t {0x80};
	inline constexpr auto select_power_down = uint8_t {0xA0};
	inline constexpr auto select_gain		= uint8_t {0xC0};

	namespace read {
		inline constexpr auto buffer_size = uint8_t {24};
	}	// namespace read

}	// namespace command

namespace channel {

	inline constexpr auto A = uint8_t {0x00};
	inline constexpr auto B = uint8_t {0x01};
	inline constexpr auto C = uint8_t {0x02};
	inline constexpr auto D = uint8_t {0x03};

}	// namespace channel

namespace data {

	namespace voltage_reference {

		inline constexpr auto Vdd = uint8_t {0x00};
		namespace internal {
			inline constexpr auto channel_A	  = uint8_t {0x08};
			inline constexpr auto channel_B	  = uint8_t {0x04};
			inline constexpr auto channel_C	  = uint8_t {0x02};
			inline constexpr auto channel_D	  = uint8_t {0x01};
			inline constexpr auto all_channel = uint8_t {0x0f};

		}	// namespace internal

	}	// namespace voltage_reference

	namespace power_mode {

		inline constexpr auto normal = uint8_t {0x00};

		namespace channel_a {
			inline constexpr auto normal		= uint8_t {0x00};
			inline constexpr auto powerDown1K	= uint8_t {0x40};
			inline constexpr auto powerDown100K = uint8_t {0x80};
			inline constexpr auto powerDown500K = uint8_t {0xC0};
		}	// namespace channel_a

		namespace channel_b {
			inline constexpr auto normal		= uint8_t {0x00};
			inline constexpr auto powerDown1K	= uint8_t {0x10};
			inline constexpr auto powerDown100K = uint8_t {0x20};
			inline constexpr auto powerDown500K = uint8_t {0x30};
		}	// namespace channel_b

		namespace channel_c {
			inline constexpr auto normal		= uint8_t {0x00};
			inline constexpr auto powerDown1K	= uint8_t {0x04};
			inline constexpr auto powerDown100K = uint8_t {0x08};
			inline constexpr auto powerDown500K = uint8_t {0x0C};
		}	// namespace channel_c

		namespace channel_d {
			inline constexpr auto normal		= uint8_t {0x00};
			inline constexpr auto powerDown1K	= uint8_t {0x01};
			inline constexpr auto powerDown100K = uint8_t {0x02};
			inline constexpr auto powerDown500K = uint8_t {0x03};
		}	// namespace channel_d

	}	// namespace power_mode

	namespace gain {

		namespace x1 {
			inline constexpr auto all = uint8_t {0x00};
		}

		namespace x2 {
			inline constexpr auto channel_A = uint8_t {0x08};
			inline constexpr auto channel_B = uint8_t {0x04};
			inline constexpr auto channel_C = uint8_t {0x02};
			inline constexpr auto channel_D = uint8_t {0x01};
			inline constexpr auto all		= uint8_t {0x0f};
		}	// namespace x2

	}	// namespace gain

}	// namespace data

}	// namespace leka::mcp4728
