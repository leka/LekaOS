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

	void setColor(RGB color) override;
	void showColor() override;

	void turnOn() override;
	void turnOff() override;

	[[nodiscard]] auto isOn() const -> bool;

  private:
	RGB _color {RGB::pure_green};
	RGB _previous_color {RGB::pure_green};
	bool _is_on {false};
	interface::SPI &_spi;
	int _n_LEDs;
	static constexpr uint8_t brightness = 0x32;

	struct frame {
		static inline auto start = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static inline auto reset = std::to_array<uint8_t>({0x00, 0x00, 0x00, 0x00});
		static inline auto end	 = std::to_array<uint8_t>({0x00, 0x00});
	};
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVER_CORE_LED_H_
