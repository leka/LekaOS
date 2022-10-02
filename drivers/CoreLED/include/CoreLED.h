// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <algorithm>

#include "RGB.h"
#include "interface/drivers/LED.h"
#include "interface/drivers/SPI.h"

namespace leka {

template <size_t NumberOfLeds>
class CoreLED : public interface::LED
{
  public:
	explicit CoreLED(interface::SPI &spi) : _spi(spi)
	{
		std::fill(_brightness.begin(), _brightness.end(), kBrightnessDefault);
	};

	void setColor(const RGB &color) override
	{
		std::fill(_colors.begin(), _colors.end(), color);
		std::fill(_brightness.begin(), _brightness.end(), kBrightnessDefault);
	}

	void setColorAtIndex(const unsigned index, const RGB &color) override
	{
		if (index >= NumberOfLeds) {
			return;
		}
		_colors.at(index)	  = color;
		_brightness.at(index) = kBrightnessDefault;
	}

	void setColorWithArray(const std::span<const RGB> color) override
	{
		std::copy(color.begin(), color.end(), _colors.begin());
		std::fill(_brightness.begin(), _brightness.end(), kBrightnessDefault);
	}

	void setColorRange(unsigned start, unsigned end, const RGB &color) override
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
		std::fill(_brightness.begin() + start, _brightness.begin() + end + 1, kBrightnessDefault);
	}

	void fadeToBlackBy(const uint8_t &value) override
	{
		for (auto &b: _brightness) {
			if (b - value <= kBrightnessMinimum) {
				b = kBrightnessMinimum;
				continue;
			}

			b = b - value;
		}
		show();
	}

	void show() override
	{
		sendAndDisplay(_colors);

		auto led_is_not_black		= [](const auto &c) { return c != RGB::black; };
		auto all_leds_are_not_black = std::all_of(_colors.begin(), _colors.end(), led_is_not_black);

		_is_color_shown = all_leds_are_not_black;
	}

	void hide() override
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
	[[nodiscard]] auto getBrightness() -> std::span<const uint8_t> override { return std::span(_brightness); }

	[[nodiscard]] auto isOn() const -> bool override { return _is_color_shown; }

  private:
	interface::SPI &_spi;
	bool _is_color_shown = false;

	std::array<RGB, NumberOfLeds> _colors;
	std::array<uint8_t, NumberOfLeds> _brightness;

	static constexpr auto kBrightnessDefault = 0xF0;
	static constexpr auto kBrightnessMinimum = 0xE0;

	struct frame {
		static constexpr auto start = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto reset = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto end	= std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
	};

	void sendAndDisplay(std::span<const RGB> colors)
	{
		_spi.write(frame::start);

		for (auto i = 0; const auto &c: colors) {
			// ? SK9822 LEDS are using BGR
			auto data = std::to_array<uint8_t>({_brightness.at(i), c.blue, c.green, c.red});
			_spi.write(data);
		}

		_spi.write(frame::reset);

		_spi.write(frame::end);
	}
};

}	// namespace leka
