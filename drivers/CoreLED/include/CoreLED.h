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
	explicit CoreLED(interface::SPI &spi, int n_LEDs) : _spi {spi}, _n_LEDs(n_LEDs) {};

	auto setColor(RGB color) -> void override;
	auto showColor() -> void override;

	auto turnOn() -> void override;
	auto turnOff() -> void override;

	[[nodiscard]] auto isOn() const -> bool;

  private:
	RGB _color {RGB::pure_green};
	RGB _previous_color {RGB::pure_green};
	bool _is_on {false};
	interface::SPI &_spi;
	int _n_LEDs;
	static constexpr uint8_t brightness = 0x32;

	std::array<uint8_t, 4> start_frame = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
	std::array<uint8_t, 4> reset_frame = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
	std::array<uint8_t, 2> end_frame   = std::to_array<uint8_t>({0x00, 0x00});
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_CORE_LED_H_
