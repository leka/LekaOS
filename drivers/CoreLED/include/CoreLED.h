// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_LED_H_
#define _LEKA_OS_DRIVER_CORE_LED_H_

#include <algorithm>

#include "bRGB.h"
#include "interface/drivers/LED.h"
#include "interface/drivers/SPI.h"

namespace leka {

template <size_t NumberOfLeds>
class CoreLED : public interface::LED
{
  public:
	explicit CoreLED(interface::SPI &spi) : _spi {spi} {};

	void setLeds(const RGB &color, const uint8_t &brightness = default_brightness) override
	{
		std::fill(_leds.begin(), _leds.end(), bRGB {brightness, color});
	}

	void setBrightness(const uint8_t &brightness) override
	{
		for (auto &l: _leds) {
			l.brightness = brightness;
		}
	}

	void reduceBrightnessBy(const uint8_t &fadeBy) override
	{
		for (auto &l: _leds) {
			l.brightness = std::max(l.brightness - fadeBy, 0xE0);
		}
	}

	void setLedsAtIndex(const unsigned index, const RGB &color, const uint8_t brightness = default_brightness) override
	{
		if (index >= NumberOfLeds) {
			return;
		}
		_leds.at(index) = bRGB {brightness, color};
	}

	void setLedsWithArray(const std::span<const bRGB> leds) override
	{
		std::copy(leds.begin(), leds.end(), _leds.begin());
	}

	void setLedsRange(unsigned start, unsigned end, const RGB &color,
					  const uint8_t &brightness = default_brightness) override
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

		std::fill(_leds.begin() + start, _leds.begin() + end + 1, bRGB {brightness, color});
	}

	void showLeds() override
	{
		sendAndDisplay(_leds);

		auto led_is_not_black		= [](const auto &c) { return (c.color != RGB::black) && (c.brightness != 0); };
		auto all_leds_are_not_black = std::all_of(_leds.begin(), _leds.end(), led_is_not_black);

		_is_color_shown = all_leds_are_not_black;
	}

	void hideColor() override
	{
		if (!isOn()) {
			return;
		}

		setBrightness(0);

		sendAndDisplay(_leds);
		_is_color_shown = false;
	}

	[[nodiscard]] auto getLeds() -> std::span<const bRGB> override { return std::span(_leds); }

	[[nodiscard]] auto isOn() const -> bool override { return _is_color_shown; }

  private:
	interface::SPI &_spi;
	bool _is_color_shown = false;

	std::array<bRGB, NumberOfLeds> _leds;

	struct frame {
		static constexpr auto start = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto reset = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto end	= std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
	};

	void sendAndDisplay(const std::array<bRGB, NumberOfLeds> &leds)
	{
		_spi.write(frame::start);

		for (const auto &l: leds) {
			// ? SK9822 LEDS are using BGR
			auto data = std::to_array<uint8_t>({l.brightness, l.color.blue, l.color.green, l.color.red});
			_spi.write(data);
		}

		_spi.write(frame::reset);

		_spi.write(frame::end);
	}
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_CORE_LED_H_
