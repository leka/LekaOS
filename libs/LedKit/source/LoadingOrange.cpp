// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LoadingOrange.h"

namespace leka::led::animation {

void LoadingOrange::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

void LoadingOrange::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void LoadingOrange::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
}

void LoadingOrange::run()
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
		default:
			break;
	}
	_belt->show();
}

void LoadingOrange::stage0()
{
	increaseBrightness();
}

void LoadingOrange::stage1()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingOrange::stage2()
{
	increaseBrightness();
}

void LoadingOrange::stage3()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingOrange::stage4()
{
	increaseBrightness();
}

void LoadingOrange::stage5()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingOrange::stage6()
{
	increaseBrightness();
}

void LoadingOrange::stage7()
{
	decreaseBrightness(0.F);
}

void LoadingOrange::increaseBrightness()
{
	static constexpr auto kMaxInputValue = uint8_t {50};
	static constexpr auto orange		 = RGB {0xFF, 0x65, 0x00};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, orange, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void LoadingOrange::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {50};
	static constexpr auto orange		 = RGB {0xFF, 0x65, 0x00};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, orange, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void LoadingOrange::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
