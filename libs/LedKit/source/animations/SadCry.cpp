// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "SadCry.h"

namespace leka::led::animation {

void SadCry::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto SadCry::isRunning() -> bool
{
	return _running;
}

void SadCry::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void SadCry::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void SadCry::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
		case 0:
			stage0();
			break;
		case 1:
			stage1();
			break;
		case 2:
			stage2();
			break;
		case 3:
			stage3();
			break;
		case 4:
			stage4();
			break;
		case 5:
			stage5();
			break;
		case 6:
			stage6();
			break;
		case 7:
			stage7();
			break;
		case 8:
			stage8();
			break;
		case 9:
			stage9();
			break;
		case 10:
			stage10();
			break;
		case 11:
			stage11();
			break;
		case 12:
			stage12();
			break;
		default:
			_running = false;
			break;
	}
	_belt->show();
}

void SadCry::stage0()
{
	static constexpr auto kMaxInputValueFirstStages = uint8_t {90};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueFirstStages); pos != 1.F) {
		RGB shade_of_blue = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt->setColor(shade_of_blue);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void SadCry::stage1()
{
	static constexpr auto kMaxInputValueFirstStages = uint8_t {90};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueFirstStages); pos != 1.F) {
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void SadCry::stage2()
{
	static constexpr auto kNumberOfLedsBelt		   = uint8_t {20};
	static constexpr auto kMaxInputValueLastStages = uint8_t {25};

	if (auto pos = utils::normalizeStep(_step, kMaxInputValueLastStages); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB {0, 128, 255}, RGB::pure_red, pos);
		_belt->setColorAtIndex(0, color);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, color);
		_step++;
	} else {
		_step = kMaxInputValueLastStages;
		_stage++;
	}
}

void SadCry::stage3()
{
	static constexpr auto kThreshold = 0.5F;
	decreaseBrightness(kThreshold);
}

void SadCry::stage4()
{
	increaseBrightness();
}

void SadCry::stage5()
{
	static constexpr auto kThreshold = 0.5F;
	decreaseBrightness(kThreshold);
}

void SadCry::stage6()
{
	increaseBrightness();
}

void SadCry::stage7()
{
	static constexpr auto kThreshold = 0.5F;
	decreaseBrightness(kThreshold);
}

void SadCry::stage8()
{
	increaseBrightness();
}

void SadCry::stage9()
{
	static constexpr auto kThreshold = 0.5F;
	decreaseBrightness(kThreshold);
}

void SadCry::stage10()
{
	increaseBrightness();
}

void SadCry::stage11()
{
	static constexpr auto kMaxInputValueLastStages = uint8_t {25};
	if (auto pos = utils::normalizeStep(_step, 2 * kMaxInputValueLastStages); pos != 1.F) {
		_step++;
	} else {
		_step = kMaxInputValueLastStages;
		_stage++;
	}
}

void SadCry::stage12()
{
	decreaseBrightness(0.F);
}

void SadCry::increaseBrightness()
{
	static constexpr auto kMaxInputValueLastStages = uint8_t {25};
	static constexpr auto kNumberOfLedsBelt		   = uint8_t {20};

	if (auto pos = utils::normalizeStep(_step, kMaxInputValueLastStages); pos != 1.F) {
		RGB shade_of_blue = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		RGB shade_of_red  = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(shade_of_blue);
		_belt->setColorAtIndex(0, shade_of_red);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, shade_of_red);
		_step++;
	} else {
		_stage++;
	}
}

void SadCry::decreaseBrightness(float threshold)
{
	static constexpr auto kMaxInputValueLastStages = uint8_t {25};
	static constexpr auto kNumberOfLedsBelt		   = uint8_t {20};

	if (auto pos = utils::normalizeStep(_step, kMaxInputValueLastStages); pos > threshold) {
		RGB shade_of_blue = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		RGB shade_of_red  = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(shade_of_blue);
		_belt->setColorAtIndex(0, shade_of_red);
		_belt->setColorAtIndex(kNumberOfLedsBelt - 1, shade_of_red);
		_step--;
	} else {
		_stage++;
	}
}

void SadCry::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
