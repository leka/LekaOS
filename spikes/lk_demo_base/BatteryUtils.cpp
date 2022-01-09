#include "BatteryUtils.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void BatteryUtils::registerEventQueue(events::EventQueue &event_queue)
{
	while (battery.getVoltage() < CoreBattery::Capacity::empty + 0.5) {
		rtos::ThisThread::sleep_for(1s);
	}

	event_queue.call_every(500ms, this, &BatteryUtils::checkReboot);
}

auto BatteryUtils::getBatteryLevel() -> float
{
	auto battery_ratio			 = 0.F;
	auto current_battery_voltage = battery.getVoltage();

	auto getBatteryLevelByQuarter = [&](float low_bound, float high_bound) {
		return ((current_battery_voltage - low_bound) / (high_bound - low_bound)) * 0.25;
	};

	if (current_battery_voltage < CoreBattery::Capacity::empty) {
		battery_ratio = 0.00F;
	} else if (current_battery_voltage < CoreBattery::Capacity::quarter) {
		battery_ratio = 0.00F + getBatteryLevelByQuarter(CoreBattery::Capacity::empty, CoreBattery::Capacity::quarter);
	} else if (current_battery_voltage < CoreBattery::Capacity::half) {
		battery_ratio = 0.25F + getBatteryLevelByQuarter(CoreBattery::Capacity::quarter, CoreBattery::Capacity::half);
	} else if (current_battery_voltage < CoreBattery::Capacity::three_quarter) {
		battery_ratio =
			0.50F + getBatteryLevelByQuarter(CoreBattery::Capacity::half, CoreBattery::Capacity::three_quarter);
	} else if (current_battery_voltage < CoreBattery::Capacity::full) {
		battery_ratio =
			0.75F + getBatteryLevelByQuarter(CoreBattery::Capacity::three_quarter, CoreBattery::Capacity::full);
	} else {
		battery_ratio = 1.00F;
	}

	return battery_ratio;
}

void BatteryUtils::checkReboot()
{
	if (battery.getVoltage() < CoreBattery::Capacity::empty) {
		NVIC_SystemReset();
	}

	if (checkRaiseAndDrop()) {
		NVIC_SystemReset();
	}

	if (was_in_user_mode && battery.isCharging()) {
		NVIC_SystemReset();
	}
}

auto BatteryUtils::checkRaiseAndDrop() -> bool
{
	if (last_charge_status != battery.isCharging()) {
		raise_and_drop_counter++;
		loop_since_charge = 0;
	} else if (battery.isCharging() and ++loop_since_charge > 120) {
		raise_and_drop_counter = 0;
	}

	last_charge_status = battery.isCharging();

	return raise_and_drop_counter > 4;
}

auto BatteryUtils::isInCharge() -> bool
{
	return battery.isCharging();
}

void BatteryUtils::setUserMode()
{
	was_in_user_mode = true;
}
