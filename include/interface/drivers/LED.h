// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_LED_H_
#define _LEKA_OS_INTERFACE_DRIVER_LED_H_

#include <span>

namespace leka {

struct RGB;

}	// namespace leka

namespace leka::interface {

class LED
{
  public:
	virtual ~LED() = default;

	virtual auto setColor(const RGB &color) -> void = 0;

	virtual auto setColorRange(unsigned start, unsigned end, const RGB &color) -> void = 0;
	virtual auto setColorAtIndex(unsigned index, const RGB &color) -> void			   = 0;
	virtual auto setColorWithArray(std::span<const RGB> color) -> void				   = 0;

	virtual auto show() -> void = 0;
	virtual auto hide() -> void = 0;

	[[nodiscard]] virtual auto getColor() -> std::span<const RGB> = 0;

	[[nodiscard]] virtual auto isOn() const -> bool = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_LED_H_
