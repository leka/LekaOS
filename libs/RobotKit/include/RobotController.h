// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_ROBOT_CONTROLLER_H_
#define _LEKA_OS_ROBOT_CONTROLLER_H_

#include "StateMachine.h"
#include "interface/RobotController.h"
#include "interface/drivers/Battery.h"

namespace leka {

template <typename sm_t = boost::sml::sm<system::robot::StateMachine>>
class RobotController : public interface::RobotController
{
  public:
	sm_t state_machine {static_cast<interface::RobotController &>(*this)};

	explicit RobotController(interface::Battery &battery) : _battery(battery) {};

	auto isCharging() -> bool final { return _battery.isCharging(); };

	void raise(auto event) { state_machine.process_event(event); };

	void registerEvents()
	{
		using namespace system::robot::sm;

		// Initializations
		// Setup callbacks for each events

		auto on_charge_did_start = [this]() { raise(event::charge_did_start {}); };
		_battery.onChargeDidStart(on_charge_did_start);

		auto on_charge_did_stop = [this]() { raise(event::charge_did_stop {}); };
		_battery.onChargeDidStop(on_charge_did_stop);

		raise(event::setup_complete {});
	};

  private:
	interface::Battery &_battery;
};

}	// namespace leka

#endif	 // _LEKA_OS_ROBOT_CONTROLLER_H_
