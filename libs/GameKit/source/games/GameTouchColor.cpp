

#include "GameTouchColor.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono;
namespace {
void do_nothing(leka::Position position) {}
}	// namespace

void GameTouchColor::start()
{
	setLedsWithRandomColors();
	auto update_touched_func = [&](Position position) { updateTouchedColor(position); };
	_touch_sensor_kit.init();
	_touch_sensor_kit.registerOnSensorTouched(update_touched_func);
	_touch_sensor_kit.start();

	_running = true;
}

void GameTouchColor::run()
{
	if (_win) {
		_touch_sensor_kit.registerOnSensorTouched(do_nothing);
		_ledManager.playReinforcer();

		if (_level == 3) {
			_running = false;
		} else {
			rtos::ThisThread::sleep_for(4s);
			_win = false;
			setLedsWithRandomColors();
			auto update_touched_func = [&](Position position) { updateTouchedColor(position); };
			_touch_sensor_kit.registerOnSensorTouched(update_touched_func);
		}
	}
}

void GameTouchColor::stop()
{
	_running = false;

	_level = 0;
	_ledManager.stop();
}

auto GameTouchColor::isRunning() const -> bool
{
	return _running;
}

void GameTouchColor::setLedsWithRandomColors()
{
	static const std::array<RGB, 4> possible_colors {RGB::pure_blue, RGB::black, RGB::pure_red, RGB::pure_green};
	uint8_t level = _level;
	if (level > possible_colors.size() - 1) {
		level = possible_colors.size() - 1;
	}

	std::array<leka::RGB, 6> colors;
	for (int i(0); i < 6; i++) {
		auto random_number = leka::utils::math::random8(0, level);

		_random_numbers.at(i) = random_number;
		colors.at(i)		  = possible_colors.at(random_number);
		_ledManager.turnOnComponent(static_cast<leka::Position>(i), colors.at(i));
	}
	log_debug("random_numbers: %d, %d , %d, %d, %d, %d", _random_numbers.at(0), _random_numbers.at(1),
			  _random_numbers.at(2), _random_numbers.at(3), _random_numbers.at(4), _random_numbers.at(5));

	// dans un for avec un cast de i en position => ferait une ligne
}
void GameTouchColor::updateTouchedColor(leka::Position component)
{
	_ledManager.turnOnComponent(component, RGB::black);
	_random_numbers.at(static_cast<int>(component)) = 255;
	if (are_all_blue_touched()) {
		_win = true;
		log_debug("win");
		++_level;
		log_debug("level %d", _level);
	}
}

auto GameTouchColor::are_all_blue_touched() -> bool
{
	for (auto number: _random_numbers) {
		if (number == 0) {
			return false;
		}
	}
	return true;
}
