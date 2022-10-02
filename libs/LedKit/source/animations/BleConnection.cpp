// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "BleConnection.h"

namespace leka::led::animation {

void BleConnection::setLeds(interface::LED &ears, interface::LED &belt)
{
	_belt = &belt;
	_ears = &ears;
}

auto BleConnection::isRunning() -> bool
{
	return _running;
}

void BleConnection::start()
{
	if (_belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step  = 0;
	_stage = 0;

	_running = true;
}

void BleConnection::stop()
{
	if (_belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void BleConnection::run()
{
	if (_belt == nullptr) {
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

		default:
			_running = false;	//
			break;
	}

	_belt->show();
}

void BleConnection::stage0()
{
	static constexpr auto max = uint8_t {20};
	increaseBrightness(max, RGB::pure_blue);
}

void BleConnection::stage1()
{
	static constexpr auto max = uint8_t {20};
	decreaseBrightness(max, RGB::pure_blue);
}

void BleConnection::stage2()
{
	static constexpr auto max = uint8_t {20};
	increaseBrightness(max, RGB::pure_blue);
}

void BleConnection::stage3()
{
	static constexpr auto max = uint8_t {20};
	decreaseBrightness(max, RGB::pure_blue);
}

void BleConnection::stage4()
{
	static const uint8_t kMaxInputValue = {30};

	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColor(RGB::pure_green);
		_step++;
	} else {
		_belt->setColor(RGB::black);
		_step = 0;
		_stage++;
	}
}
void BleConnection::increaseBrightness(uint8_t max, RGB color)
{
	static const auto kMaxInputValue = max;
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color_gradient = ColorKit::colorGradient(RGB::black, color, pos);
		_belt->setColor(color_gradient);
		_step++;

	} else {
		_step = 0;
		_stage++;
	}
}

void BleConnection::decreaseBrightness(uint8_t max, RGB color)
{
	static const auto kMaxInputValue = max;
	if (auto pos = utils::normalizeStep(kMaxInputValue - _step, max); pos != 0) {
		RGB color_gradient = ColorKit::colorGradient(RGB::black, color, pos);
		_belt->setColor(color_gradient);
		_step++;

	} else {
		_step = 0;
		_stage++;
	}
}

void BleConnection::turnLedBlack()
{
	_belt->setColor(RGB::black);
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
