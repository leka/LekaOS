// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_LED_H_
#define _LEKA_OS_DRIVER_CORE_LED_H_

#include <algorithm>

#include "RGB.h"
#include "interface/drivers/LED.h"
#include "interface/drivers/SPI.h"

namespace leka {

template <size_t NumberOfLeds>
class CoreLED : public interface::LED
{
  public:
	explicit CoreLED(interface::SPI &spi) : _spi {spi} {};

	void setColor(const RGB &color) override { std::fill(_colors.begin(), _colors.end(), color); }

	void setColorAtIndex(const int index, const RGB &color) override
	{
		if (index >= NumberOfLeds) {
			return;
		}
		_colors.at(index) = color;
	}

	void setColorWithArray(const std::span<const RGB> color) override
	{
		std::copy(color.begin(), color.end(), _colors.begin());
	}

	void setColorRange(int start, int end, const RGB &color) override
	{
		if (start >= NumberOfLeds && end >= NumberOfLeds) {
			return;
		}

		if (start > end) {
			std::swap(start, end);
		}

		if (end >= NumberOfLeds) {
			end = NumberOfLeds - 1;
		}

		std::fill(_colors.begin() + start, _colors.begin() + end + 1, color);
	}

	void showColor() override
	{
		sendAndDisplay(_colors);

		auto led_is_not_black		= [](const auto &c) { return c != RGB::black; };
		auto all_leds_are_not_black = std::all_of(_colors.begin(), _colors.end(), led_is_not_black);

		_is_color_shown = all_leds_are_not_black;
	}

	void hideColor() override
	{
		if (!isOn()) {
			return;
		}

		constexpr auto black = [] {
			auto colors = std::array<RGB, NumberOfLeds> {};
			std::fill(colors.begin(), colors.end(), RGB::black);
			return colors;
		}();

		sendAndDisplay(black);
		_is_color_shown = false;
	}

	[[nodiscard]] auto getColor() -> std::span<const RGB> override { return std::span(_colors); }

	[[nodiscard]] auto isOn() const -> bool override { return _is_color_shown; }

  private:
	interface::SPI &_spi;
	bool _is_color_shown = false;

	std::array<RGB, NumberOfLeds> _colors;

	static constexpr uint8_t brightness = 0xFF;

	struct frame {
		static constexpr auto start = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto reset = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto end	= std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
	};

	void sendAndDisplay(const std::array<RGB, NumberOfLeds> &colors)
	{
		_spi.write(frame::start);

		for (const auto &c: colors) {
			// ? SK9822 LEDS are using BGR
			auto data = std::to_array<uint8_t>({brightness, c.blue, c.green, c.red});
			_spi.write(data);
		}

		_spi.write(frame::reset);

		_spi.write(frame::end);
	}
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_CORE_LED_H_
