// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Fly.h"

namespace leka::led::animation {

static constexpr auto kThresholdDown = 0.3F;

void Fly::start()
{
	turnLedBlack();
}

void Fly::stop()
{
	turnLedBlack();
	_step  = 0;
	_stage = 0;
}

void Fly::run()
{
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
	_belt.show();
}

void Fly::stage0()
{
	static constexpr auto kInputMaxStage0 = 70;
	if (auto pos = utils::normalizeStep(_step, kInputMaxStage0); pos != 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Fly::stage1()
{
	static constexpr auto kInputMaxStage1to2 = 20;
	if (auto pos = utils::normalizeStep(_step, kInputMaxStage1to2); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Fly::stage2()
{
	static constexpr auto kInputMaxStage1to2 = 20;
	if (auto pos = utils::normalizeStep(_step, kInputMaxStage1to2); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		--_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Fly::stage3()
{
	static constexpr auto kInputMaxStage3 = 30;
	if (auto pos = utils::normalizeStep(_step, kInputMaxStage3); pos != 1.F) {
		++_step;
	} else {
		_step = 0;
		++_stage;
	}
}

void Fly::stage4()
{
	static constexpr auto kInputMaxValue4to7 = 6;
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue4to7); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Fly::stage5()
{
	static constexpr auto kInputMaxValue4to7 = 6;
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue4to7); pos >= kThresholdDown) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		--_step;
	} else {
		++_stage;
	}
}

void Fly::stage6()
{
	static constexpr auto kInputMaxValue4to7 = 6;
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue4to7); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		++_step;
	} else {
		++_stage;
	}
}

void Fly::stage7()
{
	static constexpr auto kInputMaxValue4to7 = 6;
	if (auto pos = utils::normalizeStep(_step, kInputMaxValue4to7); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		--_step;
	} else {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt.setColor(color);
		++_stage;
	}
}

void Fly::turnLedBlack()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
