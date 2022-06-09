// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START
#include "LEDAnimation.h"

namespace leka::led::animation {

struct ColorPosition {
	uint8_t position;
	RGB color;
};
class Palette
{
  public:
	Palette(const std::array<RGB, 255> &colors);
	static void blendPaletteTowardPalette(Palette &palette1, const Palette &palette2,
										  unsigned int speed = 360);   // 2nde palette ne change pas => cstPalette ref
	// valeur supérieure à 255

	auto operator[](uint8_t index) -> const RGB & { return _colors[index]; }

  private:
	std::array<RGB, 255> _colors;
};
class FastFour : public interface::LEDAnimation
{
  public:
	explicit FastFour() = default;

	void start() final;
	void run() final;
	void stop() final;

	void setLeds(interface::LED &ears, interface::LED &belt) final;
	auto isRunning() -> bool final;

  private:
	interface::LED *_ears = nullptr;
	interface::LED *_belt = nullptr;
	bool _running		  = false;
	uint8_t _step		  = 0;
	uint8_t _stage		  = 0;

	void stage0();
	void turnLedBlack();
	void animation_v3_4_12();
	void animation_v3_4_32();
	void animation_v3_5_09();
	void animation_v3_6_29();
	void animation_v3_9_18();
	void animation_v3_10_39();
	void animation_v3_12_21();

	auto rangeGradient2Colors(const RGB &color1, uint8_t value_1, const RGB &color2, uint8_t value_2,
							  const RGB &color_start = RGB::black, const RGB &color_end = RGB::white)
		-> std::array<RGB, 255>;
	auto rangeGradient3Colors(const RGB &color1, uint8_t value_1, const RGB &color2, uint8_t value_2, const RGB &color3,
							  uint8_t value_3, const RGB &color_start, const RGB &color_end) -> std::array<RGB, 255>;

	auto rangeGradient2Colors(const RGB &color_start, const RGB &color_end) -> std::array<RGB, 255>;
	auto create255RGBGradient(std::vector<ColorPosition> input_colors, const RGB &color_start = RGB::white,
							  const RGB &color_end = RGB::black) -> std::array<RGB, 255>;
	[[nodiscard]] auto random8(uint8_t min, uint8_t max) -> uint8_t;
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
