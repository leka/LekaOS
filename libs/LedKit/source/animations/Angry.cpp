// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "Angry.h"

namespace leka::led::animation {

void Angry::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Angry::isRunning() -> bool
{
	return _running;
}

void Angry::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void Angry::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void Angry::run()
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
		default:
			_running = false;
			break;
	}
	_belt->show();
}

void Angry::stage0()
{
	increaseBrightness(1.F);
}

void Angry::stage1()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void Angry::stage2()
{
	increaseBrightness(1.F);
}

void Angry::stage3()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void Angry::stage4()
{
	static constexpr auto kMaxInputValue	   = uint8_t {60};
	static constexpr auto kMaxInputValueStage6 = uint8_t {110};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_step = kMaxInputValueStage6;
		_stage++;
	}
}

void Angry::stage5()
{
	static constexpr auto kMaxInputValue	   = uint8_t {30};
	static constexpr auto kMaxInputValueStage6 = uint8_t {110};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValueStage6); pos != 0.F) {
		_step--;
	} else {
		_step = kMaxInputValue;
		_stage++;
	}
}

void Angry::stage6()
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Angry::increaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {60};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void Angry::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {60};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void Angry::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
