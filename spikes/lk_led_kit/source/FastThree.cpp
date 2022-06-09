// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "FastThree.h"

namespace leka::led::animation {

void FastThree::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto FastThree::isRunning() -> bool
{
	return _running;
}

void FastThree::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step  = 0;
	_stage = 0;

	_running = true;
}

void FastThree::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void FastThree::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			animation_v2_3_38();
			break;
		case 1:
			animation_v2_4_07();
			break;
		case 2:
			animation_v2_8_17();
			break;
		case 3:
			animation_v2_9_58();
			break;
		case 4:
			animation_v2_11_45();
			break;

		default:
			_running = false;
			break;
	}
	_ears->show();
	_belt->show();
}

void FastThree::animation_v2_3_38()
{
	constexpr auto kMaxInputValue = 60;
	if (_step == 0) {
		_belt->setColor(RGB {120, 255, 15});

		++_step;
	} else if (_step < kMaxInputValue) {
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastThree::animation_v2_4_07()
{
	constexpr auto kMaxInputValue = 60;
	if (_step == 0) {
		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(
				i, RGB {static_cast<uint8_t>(i * 20), static_cast<uint8_t>(i * 10), static_cast<uint8_t>(i * 30)});
		}
		++_step;
	} else if (_step < kMaxInputValue) {
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastThree::animation_v2_8_17()
{
	constexpr auto kMaxInputValue = 60;

	if (_step < kMaxInputValue) {
		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(i, getHsvValue(_step, 255, 255));
		}
		++_step;

	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastThree::animation_v2_9_58()
{
	constexpr auto kMaxInputValue = 60;
	static uint8_t hue			  = 0;
	if (_step < kMaxInputValue) {
		for (uint8_t i(0); i < 20; i++) {
			_belt->setColorAtIndex(i, getHsvValue(hue + (i * 10), 255, 255));
		}
		++_step;
		hue += 3;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
void FastThree::animation_v2_11_45()
{
	constexpr auto kMaxInputValue = 300;

	if (_step < kMaxInputValue) {
		_belt->setColorAtIndex(0, getHsvValue(160, random8(0, 255), random8(100, 255)));
		auto colors = _belt->getColor();
		for (uint8_t i(19); i > 0; i--) {
			_belt->setColorAtIndex(i, colors[i - 1]);
		}
		++_step;
	} else {
		_step = 0;
		turnLedBlack();
		++_stage;
	}
}
auto FastThree::getHColor(uint8_t index) const -> RGB
{
	if (index <= 51) {
		float index_color = leka::utils::math::map(index, uint8_t {0}, uint8_t {51}, 0.F, 1.F);
		return ColorKit::colorGradient(RGB::pure_red, RGB::yellow, index_color);
	} else if (index <= 102) {
		float index_color = leka::utils::math::map(index, uint8_t {52}, uint8_t {102}, 0.F, 1.F);
		return ColorKit::colorGradient(RGB::yellow, RGB::pure_green, index_color);
	} else if (index <= 153) {
		float index_color = leka::utils::math::map(index, uint8_t {103}, uint8_t {153}, 0.F, 1.F);
		return ColorKit::colorGradient(RGB::pure_green, RGB::cyan, index_color);
	} else if (index <= 204) {
		float index_color = leka::utils::math::map(index, uint8_t {154}, uint8_t {204}, 0.F, 1.F);
		return ColorKit::colorGradient(RGB::cyan, RGB::pure_blue, index_color);
	} else {
		float index_color = leka::utils::math::map(index, uint8_t {205}, uint8_t {255}, 0.F, 1.F);
		return ColorKit::colorGradient(RGB::pure_blue, RGB::magenta, index_color);
	}
}

auto FastThree::getTransparencyColor(RGB color, uint8_t index) -> RGB
{
	float index_transparency = leka::utils::math::map(index, uint8_t {0}, uint8_t {255}, 0.F, 1.F);
	return ColorKit::colorGradient(RGB::white, color, index_transparency);
}
auto FastThree::getBrightnessColor(RGB color, uint8_t index) -> RGB
{
	float index_brightness = leka::utils::math::map(index, uint8_t {0}, uint8_t {255}, 0.F, 1.F);
	return ColorKit::colorGradient(RGB::black, color, index_brightness);
}
auto FastThree::getModulateRGB(RGB color, uint8_t index_transparency, uint8_t index_brightness) -> RGB
{
	return getBrightnessColor(getTransparencyColor(color, index_transparency), index_brightness);
}
auto FastThree::getHsvValue(uint8_t index_rainbow, uint8_t index_transparency, uint8_t index_brightness) -> RGB
{
	return getBrightnessColor(getTransparencyColor(getHColor(index_rainbow), index_transparency), index_brightness);
}
auto FastThree::random8(uint8_t min, uint8_t max) -> uint8_t
{
	return min + rand() % (max - min + 1);
}

void FastThree::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
