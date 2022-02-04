// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_LED_H_
#define _LEKA_OS_INTERFACE_DRIVER_LED_H_

#include <array>
#include <span>

namespace leka {

struct RGB;

}	// namespace leka

namespace leka::interface {

template <size_t NumberOfLeds>
class LED
{
  public:
	virtual ~LED() = default;

	virtual auto setColor(const RGB &color) -> void = 0;

	virtual auto setColorRange(int start, int end, const RGB &color) -> void		   = 0;
	virtual auto setColorAtIndex(int index, const RGB &color) -> void				   = 0;
	virtual auto setColorWithArray(const std::array<RGB, NumberOfLeds> &color) -> void = 0;

	virtual auto showColor() -> void = 0;
	virtual auto hideColor() -> void = 0;

	[[nodiscard]] virtual auto getColor() -> std::span<RGB> = 0;

	[[nodiscard]] virtual auto isOn() const -> bool = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_LED_H_
