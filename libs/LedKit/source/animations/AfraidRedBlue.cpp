// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// LCOV_EXCL_START

#include "AfraidRedBlue.h"

namespace leka::led::animation {

void AfraidRedBlue::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto AfraidRedBlue::isRunning() -> bool
{
	return _running;
}

void AfraidRedBlue::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 0;
	_running = true;
}

void AfraidRedBlue::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_running = false;
}

void AfraidRedBlue::run()
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
		default:
			_running = false;
			break;
	}
	_belt->show();
}

void AfraidRedBlue::stage0()
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		_step++;
	} else {
		_step = 0;
		_stage++;
	}
}

void AfraidRedBlue::stage1()
{
	increaseBrightness(RGB::pure_red);
}

void AfraidRedBlue::stage2()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(RGB::pure_red, kTreshold);
}

void AfraidRedBlue::stage3()
{
	increaseBrightness(RGB::pure_red);
}

void AfraidRedBlue::stage4()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(RGB::pure_red, kTreshold);
}

void AfraidRedBlue::stage5()
{
	increaseBrightness(RGB::pure_red);
}

void AfraidRedBlue::stage6()
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 0.F) {
		RGB color = ColorKit::colorGradient(RGB {0, 128, 255}, RGB::pure_red, pos);
		_belt->setColor(color);
		_step--;
	} else {
		_step = 20;
		_stage++;
	}
}

void AfraidRedBlue::stage7()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(RGB {0, 128, 255}, kTreshold);
}

void AfraidRedBlue::stage8()
{
	increaseBrightness(RGB {0, 128, 255});
}

void AfraidRedBlue::stage9()
{
	static constexpr auto kTreshold = 0.4F;
	decreaseBrightness(RGB {0, 128, 255}, kTreshold);
}

void AfraidRedBlue::stage10()
{
	increaseBrightness(RGB {0, 128, 255});
}

void AfraidRedBlue::stage11()
{
	decreaseBrightness(RGB {0, 128, 255}, 0.F);
}

void AfraidRedBlue::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_ears->show();
	_belt->show();
}

void AfraidRedBlue::increaseBrightness(const RGB &color)
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB col = ColorKit::colorGradient(RGB::black, color, pos);
		_belt->setColor(col);
		_step++;
	} else {
		_stage++;
	}
}

void AfraidRedBlue::decreaseBrightness(const RGB &color, float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {20};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos > treshold) {
		RGB col = ColorKit::colorGradient(RGB::black, color, pos);
		_belt->setColor(col);
		_step--;
	} else {
		_stage++;
	}
}

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
