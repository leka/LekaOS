// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "boost/sml.hpp"
#include "interface/RobotController.h"

namespace leka::system::robot {

namespace sm::event {

	struct setup_complete {
	};
	struct sleep_timeout_did_end {
	};
	struct idle_timeout_did_end {
	};
	struct command_received {
	};
	struct charge_did_start {
	};
	struct charge_did_stop {
	};
	struct update_requested {
	};
	struct emergency_stop {
	};
	struct ble_connection {
	};
	struct ble_disconnection {
	};

}	// namespace sm::event

namespace sm::state {

	inline auto setup			  = boost::sml::state<class setup>;
	inline auto idle			  = boost::sml::state<class idle>;
	inline auto working			  = boost::sml::state<class working>;
	inline auto sleeping		  = boost::sml::state<class sleeping>;
	inline auto charging		  = boost::sml::state<class charging>;
	inline auto updating		  = boost::sml::state<class updating>;
	inline auto emergency_stopped = boost::sml::state<class emergency_stopped>;

	inline auto connected	 = boost::sml::state<class connected>;
	inline auto disconnected = boost::sml::state<class disconnected>;

}	// namespace sm::state

namespace sm::guard {

	using irc = interface::RobotController;

	struct is_charging {
		auto operator()(irc &rc) const { return rc.isCharging(); }
	};

	struct is_not_charging {
		auto operator()(irc &rc) const { return !rc.isCharging(); }
	};

	struct is_connected {
		auto operator()(irc &rc) const { return rc.isBleConnected(); }
	};

	struct is_not_connected {
		auto operator()(irc &rc) const { return !rc.isBleConnected(); }
	};

	struct is_ready_to_update {
		auto operator()(irc &rc) const { return rc.isReadyToUpdate(); }
	};

}	// namespace sm::guard

namespace sm::action {

	using irc = interface::RobotController;

	struct run_launching_behavior {
		auto operator()(irc &rc) const { rc.runLaunchingBehavior(); }
	};

	struct start_sleep_timeout {
		auto operator()(irc &rc) const { rc.startSleepTimeout(); }
	};

	struct stop_sleep_timeout {
		auto operator()(irc &rc) const { rc.stopSleepTimeout(); }
	};

	struct start_idle_timeout {
		auto operator()(irc &rc) const { rc.startIdleTimeout(); }
	};

	struct stop_idle_timeout {
		auto operator()(irc &rc) const { rc.stopIdleTimeout(); }
	};

	struct start_waiting_behavior {
		auto operator()(irc &rc) const { rc.startWaitingBehavior(); }
	};

	struct stop_waiting_behavior {
		auto operator()(irc &rc) const { rc.stopWaitingBehavior(); }
	};

	struct start_sleeping_behavior {
		auto operator()(irc &rc) const { rc.startSleepingBehavior(); }
	};

	struct stop_sleeping_behavior {
		auto operator()(irc &rc) const { rc.stopSleepingBehavior(); }
	};

	struct start_charging_behavior {
		auto operator()(irc &rc) const { rc.startChargingBehavior(); }
	};

	struct stop_charging_behavior {
		auto operator()(irc &rc) const { rc.stopChargingBehavior(); }
	};

	struct apply_update {
		auto operator()(irc &rc) const { rc.applyUpdate(); }
	};

	struct start_connection_behavior {
		auto operator()(irc &rc) const { rc.startConnectionBehavior(); }
	};

	struct start_disconnection_behavior {
		auto operator()(irc &rc) const { rc.startDisconnectionBehavior(); }
	};

	struct start_working_behavior {
		auto operator()(irc &rc) const { rc.startWorkingBehavior(); }
	};

	struct stop_actuators_and_lcd {
		auto operator()(irc &rc) const { rc.stopActuatorsAndLcd(); }
	};

	struct reset_emergency_stop_counter {
		auto operator()(irc &rc) const { rc.resetEmergencyStopCounter(); }
	};

}	// namespace sm::action

struct StateMachine {
	auto operator()() const
	{
		using namespace boost::sml;

		return make_transition_table(
			// clang-format off
			* sm::state::setup    + event<sm::event::setup_complete> [sm::guard::is_not_charging {}] = sm::state::idle
			, sm::state::setup    + event<sm::event::setup_complete> [sm::guard::is_charging {}]     = sm::state::charging
			, sm::state::setup    + boost::sml::on_exit<_>  / sm::action::run_launching_behavior {}

			, sm::state::idle     + boost::sml::on_entry<_> / (sm::action::start_sleep_timeout {}, sm::action::start_waiting_behavior {})
			, sm::state::idle     + boost::sml::on_exit<_>  / (sm::action::stop_sleep_timeout  {}, sm::action::stop_waiting_behavior  {})

			, sm::state::idle     + event<sm::event::ble_connection>                                     = sm::state::working
			, sm::state::idle     + event<sm::event::command_received> [sm::guard::is_connected {}]      = sm::state::working
			, sm::state::idle     + event<sm::event::sleep_timeout_did_end>                              = sm::state::sleeping
			, sm::state::idle     + event<sm::event::charge_did_start> [sm::guard::is_charging {}]       = sm::state::charging
			, sm::state::idle     + event<sm::event::emergency_stop>                                     = sm::state::emergency_stopped

			, sm::state::working + boost::sml::on_entry<_> / (sm::action::start_idle_timeout {}, sm::action::start_working_behavior {})
			, sm::state::working + boost::sml::on_exit<_>  / sm::action::stop_idle_timeout {}

			, sm::state::working  + event<sm::event::ble_disconnection>                                  = sm::state::idle
			, sm::state::working  + event<sm::event::idle_timeout_did_end>                               = sm::state::idle
			, sm::state::working  + event<sm::event::charge_did_start> [sm::guard::is_charging {}]       = sm::state::charging
			, sm::state::working  + event<sm::event::emergency_stop>                                     = sm::state::emergency_stopped

			, sm::state::sleeping + boost::sml::on_entry<_> / sm::action::start_sleeping_behavior {}
			, sm::state::sleeping + boost::sml::on_exit<_>  / sm::action::stop_sleeping_behavior {}

			, sm::state::sleeping + event<sm::event::command_received>    [sm::guard::is_connected {}]   = sm::state::working
			, sm::state::sleeping + event<sm::event::ble_connection>                                     = sm::state::working
			, sm::state::sleeping + event<sm::event::charge_did_start>    [sm::guard::is_charging {}]    = sm::state::charging
			, sm::state::sleeping + event<sm::event::emergency_stop>                                     = sm::state::emergency_stopped

			, sm::state::charging + boost::sml::on_entry<_> / sm::action::start_charging_behavior {}
			, sm::state::charging + boost::sml::on_exit<_>  / sm::action::stop_charging_behavior {}

			, sm::state::charging + event<sm::event::charge_did_stop>  [sm::guard::is_not_charging {} && sm::guard::is_not_connected {}]  = sm::state::idle
			, sm::state::charging + event<sm::event::charge_did_stop>  [sm::guard::is_not_charging {} && sm::guard::is_connected {}]      = sm::state::working
			, sm::state::charging + event<sm::event::update_requested> [sm::guard::is_ready_to_update {}]                                 = sm::state::updating
			, sm::state::charging + event<sm::event::ble_connection>                                                                      = sm::state::charging
			, sm::state::charging + event<sm::event::ble_disconnection>                                                                   = sm::state::charging
			, sm::state::charging + event<sm::event::command_received>                                                                    = sm::state::charging
			, sm::state::charging + event<sm::event::emergency_stop>                                                                      = sm::state::emergency_stopped

			, sm::state::updating + boost::sml::on_entry<_> / sm::action::apply_update {}

			, sm::state::emergency_stopped + boost::sml::on_entry<_> / sm::action::stop_actuators_and_lcd {}
			, sm::state::emergency_stopped + boost::sml::on_exit <_> / sm::action::reset_emergency_stop_counter {}

			, sm::state::emergency_stopped + event<sm::event::command_received> [sm::guard::is_not_charging {} && sm::guard::is_connected {}] = sm::state::working
			, sm::state::emergency_stopped + event<sm::event::ble_connection>   [sm::guard::is_not_charging {}]                               = sm::state::working
			, sm::state::emergency_stopped + event<sm::event::charge_did_start> [sm::guard::is_charging {}]                                   = sm::state::charging
			, sm::state::emergency_stopped + event<sm::event::command_received> [sm::guard::is_charging {} && sm::guard::is_connected {}]     = sm::state::charging
			, sm::state::emergency_stopped + event<sm::event::ble_connection>   [sm::guard::is_charging {}]                                   = sm::state::charging

			,

			* sm::state::disconnected + event<sm::event::ble_connection>    / sm::action::start_connection_behavior {}    = sm::state::connected
			, sm::state::connected    + event<sm::event::ble_disconnection> / sm::action::start_disconnection_behavior {} = sm::state::disconnected
			// clang-format on
		);
	}
};

}	// namespace leka::system::robot
