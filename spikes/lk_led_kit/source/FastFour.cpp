// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "FastFour.h"
#include <cstdint>

namespace leka::led::animation {

void FastFour::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto FastFour::isRunning() -> bool
{
	return _running;
}

void FastFour::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step  = 0;
	_stage = 0;

	_running = true;
}

void FastFour::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void FastFour::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			animation_v3_4_12();
			break;
		case 1:
			animation_v3_4_32();
			break;
		case 2:
			animation_v3_5_09();
			break;
		case 3:
			animation_v3_6_29();
			break;
		case 4:
			animation_v3_9_18();
			break;
		case 5:
			animation_v3_10_39();
			break;
		case 6:
			animation_v3_12_21();
			break;
		default:
			_running = false;
			break;
	}
	_ears->show();
	_belt->show();
}

void FastFour::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}
void FastFour::animation_v3_4_12()
{
	std::vector<ColorPosition> input_colors = {
		{0, 0, 0, 0}, {128, 255, 0, 0}, {200, 255, 255, 0}, {255, 255, 255, 255}};

	static Palette p1(create255RGBGradient(input_colors));
	constexpr auto kMaxInputValue = 60;
	constexpr auto pas			  = 255.F / 19;
	float cpt					  = 0;
	if (_step < kMaxInputValue) {
		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(i, p1[static_cast<uint8_t>(cpt)]);
			cpt += pas;
		}
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastFour::animation_v3_4_32()
{
	std::vector<ColorPosition> input_colors = {{128, 255, 0, 0}, {200, 255, 255, 0}};
	static Palette p1(create255RGBGradient(input_colors));
	constexpr auto kMaxInputValue = 60;
	constexpr auto pas			  = 255.F / 19;
	float cpt					  = 0;
	if (_step < kMaxInputValue) {
		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(i, p1[static_cast<uint8_t>(cpt + _step * pas)]);
			cpt += pas;
		}
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastFour::animation_v3_5_09()
{
	std::vector<ColorPosition> violet_input_colors = {
		{79, 255, 0, 255}, {111, 250, 240, 230}, {143, 255, 0, 255}, {159, 148, 0, 211}};

	static Palette p1(create255RGBGradient(violet_input_colors, RGB {148, 0, 211}, RGB {250, 240, 230}));
	constexpr auto kMaxInputValue = 80;
	constexpr auto pas			  = 255.F / 19;
	float cpt					  = 0;
	if (_step < kMaxInputValue) {
		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(i, p1[static_cast<uint8_t>(cpt + _step * pas)]);
			cpt += pas;
		}
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastFour::animation_v3_6_29()
{
	std::vector<ColorPosition> input_violet_colors = {
		{79, 255, 0, 255}, {111, 250, 240, 230}, {143, 255, 0, 255}, {159, 148, 0, 211}};

	static Palette p1(create255RGBGradient(input_violet_colors, RGB {148, 0, 211}, RGB {250, 240, 230}));
	constexpr auto kMaxInputValue = 150;

	if (_step < kMaxInputValue) {
		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(random8(0, 19), p1[random8(0, 255)]);
		}
		_belt->fadeToBlackBy(1);
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastFour::animation_v3_9_18()
{
	std::vector<ColorPosition> input_green_blue_colors = {
		{0, 0, 255, 245}, {46, 0, 21, 255}, {179, 12, 250, 0}, {250, 0, 255, 245}};
	Palette p3(create255RGBGradient(input_green_blue_colors));
	static std::array<uint8_t, 20> colorIndex;
	for (uint8_t i(0); i < 20; i++) {
		colorIndex[i] = random8(0, 255);
	}
	constexpr auto kMaxInputValue = 80;

	if (_step < kMaxInputValue) {
		for (uint8_t i(0); i < 20; i++) {
			if (_step % 2 == 0) {
				colorIndex[i]++;
			}
			_belt->setColorAtIndex(i, p3[colorIndex[i]]);
		}
		++_step;

	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastFour::animation_v3_10_39()
{
	std::vector<ColorPosition> input_sunset_colors = {{0, 120, 0, 0},	 {22, 179, 22, 0},	 {51, 255, 104, 0},
													  {85, 167, 22, 18}, {135, 100, 0, 103}, {198, 16, 0, 130},
													  {255, 0, 0, 160}};
	Palette p4(create255RGBGradient(input_sunset_colors));
	std::array<uint8_t, 20> colorIndex;
	for (int i(0); i < 20; i++) {
		colorIndex[i] = random8(0, 255);
	}
	constexpr auto kMaxInputValue = 200;
	constexpr auto pas			  = 255.F / 19;
	float cpt					  = 0;

	if (_step < kMaxInputValue) {
		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(i, p4[static_cast<uint8_t>(cpt + _step * pas)]);
			cpt += pas;
		}
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastFour::animation_v3_12_21()
{
	constexpr auto pas = 255.F / 19;
	float cpt		   = 0;
	std::array<uint8_t, 20> colorIndex;
	for (uint8_t i(0); i < 20; i++) {
		colorIndex[i] = random8(0, 255);
	}
	static std::array<uint8_t, 20> colorIndexcopy		 = colorIndex;
	std::vector<ColorPosition> colors_green_blue_palette = {
		{0, 0, 194, 255}, {46, 3, 0, 246}, {176, 55, 222, 70}, {255, 0, 194, 255}};

	std::vector<ColorPosition> colors_orange_pink_palette = {
		{0, 255, 100, 0}, {90, 255, 0, 255}, {150, 255, 100, 0}, {255, 255, 100, 0}};

	std::vector<ColorPosition> colors_brown_green_palette = {
		{0, 6, 255, 0}, {71, 0, 255, 153}, {122, 200, 200, 200}, {181, 110, 61, 6}, {255, 6, 255, 0}};
	static std::array<RGB, 255> array_gb = create255RGBGradient(colors_green_blue_palette);
	static std::array<RGB, 255> array_op = create255RGBGradient(colors_orange_pink_palette);

	static Palette gbpal(array_gb);
	static Palette oppal(array_op);
	static Palette bgpal(create255RGBGradient(colors_brown_green_palette));

	static Palette currentPalette(array_gb);
	Palette *targetPalette;
	static uint8_t whichPalette	  = 0;
	constexpr auto kMaxInputValue = 400;

	if (_step < kMaxInputValue) {
		switch (whichPalette) {
			case 0:
				targetPalette = &oppal;
				break;
			case 1:
				targetPalette = &bgpal;
				break;
			case 2:
				targetPalette = &gbpal;
				break;
			default:
				break;
		}
		Palette::blendPaletteTowardPalette(currentPalette, *targetPalette, 255);

		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(i, currentPalette[colorIndexcopy[i]]);
		}

		if ((_step / 30) % 3 == 0) {
			whichPalette = 0;
		} else if ((_step / 30) % 3 == 1) {
			whichPalette = 1;
		} else if ((_step / 30) % 3 == 2) {
			whichPalette = 2;
		}
		++_step;

	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}

Palette::Palette(const std::array<RGB, 255> &colors) : _colors(colors) {}
void Palette::blendPaletteTowardPalette(Palette &palette1, const Palette &palette2, unsigned int speed)
{
	uint8_t *p1;
	uint8_t *p2;
	unsigned int changes = 0;

	p1 = (uint8_t *)palette1._colors.data();
	p2 = (uint8_t *)palette2._colors.data();

	const unsigned int totalChannels = 765;
	for (uint8_t i(0); i < totalChannels; i++) {
		if (p1[i] == p2[i]) {
			continue;
		}

		if (p1[i] < p2[i]) {
			p1[i]++;
			changes++;
		}

		else if (p1[i] > p2[i]) {
			p1[i]--;
			changes++;
			if (p1[i] > p2[i]) {
				p1[i]--;
			}
		}
	}
}

auto FastFour::rangeGradient2Colors(const RGB &color_start, const RGB &color_end) -> std::array<RGB, 255>
{
	std::array<RGB, 255> colors;
	for (uint8_t index(0); index < 255; index++) {
	}
	return colors;
}

auto FastFour::rangeGradient2Colors(const RGB &color1, uint8_t value_1, const RGB &color2, uint8_t value_2,
									const RGB &color_start, const RGB &color_end) -> std::array<RGB, 255>
{
	std::array<RGB, 255> colors;
	for (uint8_t index(0); index < value_1; index++) {
		float index_color = leka::utils::math::map(index, uint8_t {0}, uint8_t {value_1}, 0.F, 1.F);
		colors[index]	  = ColorKit::colorGradient(color_start, color1, index_color);
	}
	for (uint8_t index(value_1); index < value_2; index++) {
		float index_color = leka::utils::math::map(index, uint8_t {value_1}, uint8_t {value_2}, 0.F, 1.F);
		colors[index]	  = ColorKit::colorGradient(color1, color2, index_color);
	}
	for (uint8_t index(value_2); index < 255; index++) {
		float index_color = leka::utils::math::map(index, uint8_t {value_2}, uint8_t {254}, 0.F, 1.F);
		colors[index]	  = ColorKit::colorGradient(color2, color_end, index_color);
	}
	return colors;
}

auto FastFour::rangeGradient3Colors(const RGB &color1, uint8_t value_1, const RGB &color2, uint8_t value_2,
									const RGB &color3, uint8_t value_3, const RGB &color_start, const RGB &color_end)
	-> std::array<RGB, 255>
{
	std::array<RGB, 255> colors;
	for (uint8_t index(0); index < value_1; index++) {
		float index_color = leka::utils::math::map(index, uint8_t {0}, uint8_t {value_1}, 0.F, 1.F);
		colors[index]	  = ColorKit::colorGradient(color_start, color1, index_color);
	}
	for (uint8_t index(value_1); index < value_2; index++) {
		float index_color = leka::utils::math::map(index, uint8_t {value_1}, uint8_t {value_2}, 0.F, 1.F);
		colors[index]	  = ColorKit::colorGradient(color1, color2, index_color);
	}
	for (uint8_t index(value_2); index < value_3; index++) {
		float index_color = leka::utils::math::map(index, uint8_t {value_2}, uint8_t {value_3}, 0.F, 1.F);
		colors[index]	  = ColorKit::colorGradient(color2, color3, index_color);
	}

	for (uint8_t index(value_3); index < 255; index++) {
		float index_color = leka::utils::math::map(index, uint8_t {value_3}, uint8_t {254}, 0.F, 1.F);
		colors[index]	  = ColorKit::colorGradient(color2, color3, index_color);
	}
	return colors;
}

auto FastFour::create255RGBGradient(std::vector<ColorPosition> input_colors, const RGB &color_start,
									const RGB &color_end) -> std::array<RGB, 255>
{
	std::array<RGB, 255> return_colors;
	auto size		 = static_cast<uint8_t>(input_colors.size());
	auto first_input = input_colors[0];

	auto last_input = input_colors[size - 1];

	for (uint8_t index(0); index < first_input.position; index++) {
		float index_color	 = leka::utils::math::map(index, uint8_t {0}, uint8_t {first_input.position}, 0.F, 1.F);
		return_colors[index] = ColorKit::colorGradient(color_start, first_input.color, index_color);
	}

	for (uint8_t i(0); i < size - 1; i++) {
		for (uint8_t index(input_colors[i].position); index < input_colors[i + 1].position; index++) {
			float index_color = leka::utils::math::map(index, uint8_t {input_colors[i].position},
													   uint8_t {input_colors[i + 1].position}, 0.F, 1.F);
			return_colors[index] =
				ColorKit::colorGradient(input_colors[i].color, input_colors[i + 1].color, index_color);
		}
	}

	for (uint8_t index(last_input.position); index < 255; index++) {
		float index_color	 = leka::utils::math::map(index, uint8_t {last_input.position}, uint8_t {255}, 0.F, 1.F);
		return_colors[index] = ColorKit::colorGradient(last_input.color, color_end, index_color);
	}

	return return_colors;
}
auto FastFour::random8(uint8_t min, uint8_t max) -> uint8_t
{
	return min + rand() % (max - min + 1);
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
