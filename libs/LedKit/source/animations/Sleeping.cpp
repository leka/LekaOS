// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Sleeping.h"

namespace leka::led::animation {

void Sleeping::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Sleeping::isRunning() -> bool
{
	return _running;
}

void Sleeping::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Sleeping::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Sleeping::run()
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
			_running = false;
			break;
	}
	_belt->show();
}

void Sleeping::stage0()
{
	static constexpr auto kMaxInputValueStage0 = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage0); pos != 1.F) {
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Sleeping::stage1()
{
	static constexpr auto kMaxInputValueStage1 = uint8_t {40};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage1); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void Sleeping::stage2()
{
	static constexpr auto kMaxInputValueStage2	 = uint8_t {20};
	static constexpr auto kMaxInputValueStage4_6 = uint8_t {45};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage2); pos != 1.F) {
		RGB color = RGB::white;
		_belt->setColor(color);
		_step++;
	} else {
		_step = kMaxInputValueStage4_6;
		_stage++;
	}
}

void Sleeping::stage3()
{
	static constexpr auto kTreshold1 = 0.45F;
	decreaseBrightness(kTreshold1);
}

void Sleeping::stage4()
{
	static constexpr auto kTreshold2 = 0.9F;
	increaseBrightness(kTreshold2);
}

void Sleeping::stage5()
{
	static constexpr auto kTreshold3 = 0.45F;
	decreaseBrightness(kTreshold3);
}

void Sleeping::stage6()
{
	static constexpr auto kTreshold4 = 0.80F;
	increaseBrightness(kTreshold4);
}

void Sleeping::stage7()
{
	decreaseBrightness(0.F);
}

void Sleeping::increaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValueStage3to7 = uint8_t {45};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage3to7); pos < treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void Sleeping::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValueStage3to7 = uint8_t {45};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage3to7); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Sleeping::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
