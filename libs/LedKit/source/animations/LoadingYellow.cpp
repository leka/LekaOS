// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LoadingYellow.h"

namespace leka::led::animation {

void LoadingYellow::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

void LoadingYellow::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void LoadingYellow::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
}

void LoadingYellow::run()
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
		default:
			break;
	}
	_belt->show();
}

void LoadingYellow::stage0()
{
	increaseBrightness();
}

void LoadingYellow::stage1()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingYellow::stage2()
{
	increaseBrightness();
}

void LoadingYellow::stage3()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingYellow::stage4()
{
	increaseBrightness();
}

void LoadingYellow::stage5()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingYellow::stage6()
{
	increaseBrightness();
}

void LoadingYellow::stage7()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(kTreshold);
}

void LoadingYellow::stage8()
{
	increaseBrightness();
}

void LoadingYellow::stage9()
{
	decreaseBrightness(0.F);
}

void LoadingYellow::increaseBrightness()
{
	static constexpr auto kMaxInputValue = uint8_t {25};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::yellow, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void LoadingYellow::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {25};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::yellow, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void LoadingYellow::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
