// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "AfraidBlue.h"

namespace leka::led::animation {

void AfraidBlue::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto AfraidBlue::isRunning() -> bool
{
	return _running;
}

void AfraidBlue::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void AfraidBlue::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void AfraidBlue::run()
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
	_ears->show();
	_belt->show();
}

void AfraidBlue::stage0()
{
	increaseBrightness();
}

void AfraidBlue::stage1()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AfraidBlue::stage2()
{
	increaseBrightness();
}

void AfraidBlue::stage3()
{
	static constexpr auto kTreshold = 0.7F;
	decreaseBrightness(kTreshold);
}

void AfraidBlue::stage4()
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt->setColor(color);
		_ears->setColor(color);
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void AfraidBlue::stage5()
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_belt->setColor(RGB {0, 128, 255});
		_ears->setColor(RGB {0, 128, 255});
		_step++;
	} else {
		_stage++;
	}
}

void AfraidBlue::stage6()
{
	decreaseBrightness(0.F);
}

void AfraidBlue::increaseBrightness()
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt->setColor(color);
		_ears->setColor(color);
		_step++;
	} else {
		_stage++;
	}
}

void AfraidBlue::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {30};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {0, 128, 255}, pos);
		_belt->setColor(color);
		_ears->setColor(color);
		_step--;
	} else {
		_stage++;
	}
}

void AfraidBlue::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
