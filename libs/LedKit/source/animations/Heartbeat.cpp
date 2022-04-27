// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? LCOV_EXCL_START - Exclude from coverage report

#include "Heartbeat.h"

#include "rtos/ThisThread.h"

#include "LogKit.h"
using namespace std::chrono;
namespace leka::led::animation {

void Heartbeat::setLeds(interface::LED &ears, interface::LED &belt)
{
	_ears = &ears;
	_belt = &belt;
}

auto Heartbeat::isRunning() -> bool
{
	return _running;
}

void Heartbeat::start()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	turnLedBlack();
	_step	 = 0;
	_stage	 = 1;
	_running = true;
}

void Heartbeat::stop()
{
	if (_ears == nullptr || _belt == nullptr) {
		logger::init();
		log_debug("je ne connais pas de leds");
		return;
	}

	turnLedBlack();
	_running = false;
}

void Heartbeat::run()
{
	if (_ears == nullptr || _belt == nullptr) {
		return;
	}

	switch (_stage) {
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
			// rtos::ThisThread::sleep_for(1000ms);
		default:
			_running = false;	//
			break;
	}

	_belt->show();
}

void Heartbeat::stage1()
{
	static constexpr auto kMaxInputValue = uint8_t {5};	  // avec çà on a stage1 qui dure 1 s
	// le Kmax est le 34 et le 1 c'est le step (il change) dans 1/34 que vaut pos. NB: F = flottant
	// jouer sur Kmax va changer le nombre d'itérations
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {255, 0, 0}, pos);
		_belt->setColor(color);
		_ears->setColor(color);
		_belt->show();
		_step++;
		rtos::ThisThread::sleep_for(10);
		log_debug("stage1");
		log_debug(" stage1: _step %i", _step);
	} else {
		_step = 0;

		_stage++;
	}
}

void Heartbeat::stage2()
{
	static constexpr auto kMaxInputValue = uint8_t {3};
	if (auto pos = utils::normalizeStep(kMaxInputValue - _step, kMaxInputValue); pos != 0) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(color);
		_belt->show();
		log_debug("corps du if de decrease_brightness");
		_step++;

	} else {
		log_debug("je passe au stage suivant");
		_stage++;
	}
}

void Heartbeat::stage3()
{
	static constexpr auto kMaxInputValue = uint8_t {3};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB {255, 0, 0}, pos);
		_belt->setColor(color);
		_ears->setColor(color);
		_belt->show();
		_step++;
		rtos::ThisThread::sleep_for(10);
		log_debug("stage3");
	} else {
		_step = 0;
		_stage++;
	}
}

void Heartbeat::stage4()
{
	static constexpr auto kTreshold = 0.F;
	decreaseBrightness(kTreshold);
	log_debug("stage4");
}

void Heartbeat::increaseBrightness()
{
	static constexpr auto kMaxInputValue = uint8_t {34};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos != 1.F) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(color);
		_belt->show();
		_step++;
	} else {
		_stage++;
	}
}

void Heartbeat::decreaseBrightness(float treshold)
{
	static constexpr auto kMaxInputValue = uint8_t {3};
	if (auto pos = utils::normalizeStep(_step, kMaxInputValue); pos > treshold) {
		RGB color = ColorKit::colorGradient(RGB::black, RGB::pure_red, pos);
		_belt->setColor(color);
		_belt->show();
		log_debug("corps du if de decrease_brightness");
		_step--;

	} else {
		log_debug("je passe au stage suivant");
		_stage++;
	}
}
// le Kmax est le 34 et le 1 c'est le step (il change) dans 1/34 que vaut pos. NB: F = flottant
// jouer sur Kmax va changer le nombre d'itérations
void Heartbeat::turnLedBlack()
{
	_ears->setColor(RGB::black);
	_belt->setColor(RGB::black);
	_belt->show();
}

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
