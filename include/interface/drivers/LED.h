// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_LED_H_
#define _LEKA_OS_INTERFACE_DRIVER_LED_H_

#include <cstdint>
#include <span>

namespace leka {

struct RGB;
struct bRGB;

}	// namespace leka

namespace leka::interface {

class LED
{
  public:
	virtual ~LED() = default;

	static constexpr uint8_t default_brightness = 0xF0;

	virtual auto setLeds(const RGB &color, const uint8_t &brightness = default_brightness) -> void = 0;
	virtual auto setBrightness(const uint8_t &brightness) -> void								   = 0;
	virtual auto reduceBrightnessBy(const uint8_t &fadeBy) -> void								   = 0;

	virtual auto setLedsRange(unsigned start, unsigned end, const RGB &color,
							  const uint8_t &brightness = default_brightness) -> void							   = 0;
	virtual auto setLedsAtIndex(unsigned index, const RGB &color, uint8_t brightness = default_brightness) -> void = 0;
	virtual auto setLedsWithArray(std::span<const bRGB> leds) -> void											   = 0;

	virtual auto showLeds() -> void	 = 0;
	virtual auto hideColor() -> void = 0;

	[[nodiscard]] virtual auto getLeds() -> std::span<const bRGB> = 0;

	[[nodiscard]] virtual auto isOn() const -> bool = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_LED_H_
