// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_LED_H_
#define _LEKA_OS_INTERFACE_DRIVER_LED_H_

#include <array>
#include <cstddef>
#include <cstdint>

namespace leka {

struct RGB;

}	// namespace leka

namespace leka::interface {

template <size_t NumberOfLeds>
class LED
{
  public:
	virtual ~LED() = default;

	virtual auto setColor(RGB color) -> void									= 0;
	virtual auto setColorAtIndex(int index, RGB color) -> void					= 0;
	virtual auto setColorWithArray(std::array<RGB, NumberOfLeds> color) -> void = 0;
	virtual auto showColor() -> void											= 0;
	virtual auto hideColor() -> void											= 0;

	virtual auto isOn() -> bool = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_LED_H_
