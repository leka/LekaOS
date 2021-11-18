// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LEDS_UTILS_H_
#define _LEKA_OS_LEDS_UTILS_H_

#include "FastLED.h"

namespace leka {

enum class LedsReinforcer : uint8_t
{
	green,
	violet_green_blink,
	fire,
	glitters,
	rainbow
};

enum class LedsRange
{
	front_left	= 0x01,
	back_left	= 0x02,
	back_right	= 0x04,
	front_right = 0x08,
	ear_left	= 0x10,
	ear_right	= 0x20,
	belt_left	= front_left + back_left,
	belt_right	= front_right + back_right,
	belt		= belt_left + belt_right,
	ears		= ear_right + ear_left,
	all			= ears + belt
};

inline LedsRange operator|(LedsRange lhs, LedsRange rhs)
{
	return static_cast<LedsRange>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline LedsRange operator&(LedsRange lhs, LedsRange rhs)
{
	return static_cast<LedsRange>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

class LedsUtils
{
  public:
	explicit LedsUtils() = default;

	void initialize();
	void initializationAnimation();

	void setBrightness(uint8_t brightness);

	void turnOff(LedsRange range);
	void turnOn(LedsRange range, CRGB color);

	void pulsation(CRGB color = CRGB::White);

	void runReinforcer(LedsReinforcer reinforcer);

  private:
	void runPaletteColors(CRGBPalette16 palette);
	void runGlitterReinforcer();
	void runGreenReinforcer();
	void runVioletGreenBlinkReinforcer();

	volatile uint8_t _brightness = 0;

	static const int leds_belt_count {20};
	static const int leds_ears_count {2};

	mbed::SPI leds_belt_output {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
	mbed::SPI leds_ears_output {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};

	std::array<CRGB, leds_belt_count> leds_belt_control;
	std::array<CRGB, leds_ears_count> leds_ears_control;

	CFastLED leds_controller;

	CRGBPalette16 _palette {};
	TBlendType _blending {LINEARBLEND};
};

}	// namespace leka

#endif	 // _LEKA_OS_LEDS_UTILS_H_
