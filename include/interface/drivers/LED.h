// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_LED_H_
#define _LEKA_OS_INTERFACE_DRIVER_LED_H_

#include <cstdint>

namespace leka {

struct LEDColor {
    uint8_t red {};
    uint8_t green {};
    uint8_t blue {};

    [[nodiscard]] auto getRGB() const -> uint32_t { return red << 16 | green << 8 | blue; }

	auto operator==(LEDColor const& a) const -> bool
	{
		return (this->red == a.red && this->green == a.green && this -> blue == a.blue);
	}

    static const LEDColor white;
    static const LEDColor black;
    static const LEDColor pure_red;
    static const LEDColor pure_green;
    static const LEDColor pure_blue;
    static const LEDColor yellow;
    static const LEDColor cyan;
    static const LEDColor magenta;
};


constexpr LEDColor LEDColor::white {0xFF, 0xFF, 0xFF};
constexpr LEDColor LEDColor::black {0x00, 0x00, 0x00};

constexpr LEDColor LEDColor::pure_red {0xFF, 0x00, 0x00};
constexpr LEDColor LEDColor::pure_green {0x00, 0xFF, 0x00};
constexpr LEDColor LEDColor::pure_blue {0x00, 0x00, 0xFF};

constexpr LEDColor LEDColor::yellow {0xFF, 0xFF, 0x00};
constexpr LEDColor LEDColor::cyan {0x00, 0xFF, 0xFF};
constexpr LEDColor LEDColor::magenta {0xFF, 0x00, 0xFF};

}   // namespace leka

namespace leka::interface {

class LED
{
  public:
    virtual ~LED() = default;

    virtual auto setBeltColor(LEDColor color) -> void = 0;
	virtual auto getBeltColor() -> LEDColor = 0;
    virtual auto showColor() -> void = 0;
    virtual auto turnOn() -> void = 0;
    virtual auto turnOff() -> void = 0;
};

}   // namespace leka::interface

#endif   // _LEKA_OS_INTERFACE_DRIVER_LED_H_


