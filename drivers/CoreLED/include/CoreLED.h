// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_LED_H_
#define _LEKA_OS_DRIVER_CORE_LED_H_

#include "RGB.h"
#include "interface/drivers/LED.h"
#include "interface/drivers/SPI.h"

namespace leka {

class CoreLED : public interface::LED
{
  public:
	explicit CoreLED(interface::SPI &spi, int size) : _spi {spi}, _size(size) {};

	void setColor(RGB color) override;
	void showColor() override;

	void turnOn() override;
	void turnOff() override;

	[[nodiscard]] auto isOn() const -> bool;

  private:
	interface::SPI &_spi;
	int _size;

	RGB _color {RGB::black};
	RGB _previous_color {RGB::black};

	bool _is_on {false};

	static constexpr uint8_t brightness = 0x32;

	struct frame {
		static constexpr auto start = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto reset = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static constexpr auto end	= std::to_array<uint8_t>({0x00, 0x00});
	};
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_CORE_LED_H_
