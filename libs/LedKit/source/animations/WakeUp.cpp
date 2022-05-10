// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "WakeUp.h"

namespace leka::led::animation {

void WakeUp::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto WakeUp::isRunning() -> bool
{
	return _running;
}

void WakeUp::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void WakeUp::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void WakeUp::run()
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

void WakeUp::stage0()
{
	static constexpr auto kMaxInputValue = uint8_t {24};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void WakeUp::stage1()
{
	static constexpr auto kTreshold1 = 0.80F;
	increaseBrightness(kTreshold1);
}

void WakeUp::stage2()
{
	static constexpr auto kTreshold2 = 0.45F;
	decreaseBrightness(kTreshold2);
}

void WakeUp::stage3()
{
	static constexpr auto kTreshold3 = 0.9F;
	increaseBrightness(kTreshold3);
}

void WakeUp::stage4()
{
	static constexpr auto kTreshold4 = 0.5F;
	decreaseBrightness(kTreshold4);
}

void WakeUp::stage5()
{
	static constexpr auto kMaxInputValue = uint8_t {24};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void WakeUp::stage6()
{
	static constexpr auto kMaxInputValue1 = uint8_t {6};
	static constexpr auto kMaxInputValue2 = uint8_t {35};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue1); pos != 1.F) {
		RGB color = RGB::white;
		_belt->setColor(color);
		_step++;
	} else {
		_step = kMaxInputValue2;
		_stage++;
	}
}

void WakeUp::stage7()
{
	static constexpr auto kMaxInputValue = uint8_t {35};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void WakeUp::increaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {24};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos < treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void WakeUp::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {24};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::white, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void WakeUp::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
