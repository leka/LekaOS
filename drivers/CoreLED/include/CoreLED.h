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
class CoreLED : public interface::LED<NumberOfLeds>
{
  public:
	explicit CoreLED(interface::SPI &spi) : _spi {spi} {};

	void setColor(RGB color) override { std::fill(_color.begin(), _color.end(), color); }

	void setColorAtIndex(int index, RGB color) override { _color.at(index) = color; }

	void setColorWithArray(std::array<RGB, NumberOfLeds> color) override
	{
		std::copy(color.begin(), color.end(), _color.begin());
	}

	void showColor() override
	{
		sendAndDisplay(_color);

		auto led_is_not_black		= [](auto c) { return c != RGB::black; };
		auto all_leds_are_not_black = std::all_of(_color.begin(), _color.end(), led_is_not_black);

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

	auto isOn() -> bool override { return _is_color_shown; }

  private:
	interface::SPI &_spi;
	bool _is_color_shown = false;

	std::array<RGB, NumberOfLeds> _color;

	static constexpr uint8_t brightness = 0xFF;

	struct frame {
		static constexpr auto start = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto reset = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto end	= std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
	};

	void sendAndDisplay(std::array<RGB, NumberOfLeds> colors)
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
