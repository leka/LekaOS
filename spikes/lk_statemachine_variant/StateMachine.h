// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_STATE_MACHINE_
#define _LEKA_OS_STATE_MACHINE_

#include <cstdint>
#include <variant>

#include "EventFlags.h"

namespace leka {

enum class Events : uint32_t
{
	NONE	= 0,
	WAKEUP	= (1UL << 0),
	TIMEOUT = (1UL << 1),
	ALL		= 0xFFFFFFFF
};

namespace interface {
	class State
	{
	  public:
		virtual auto processEvent(Events event) -> State { return State(); };
	};
}	// namespace interface

class WaitingForCommands : public interface::State
{
  public:
	auto processEvent(Events event) -> interface::State final;
};

class Sleep : public interface::State
{
  public:
	auto processEvent(Events event) -> interface::State final;
};

class RobotState
{
  public:
	RobotState() = default;

	void StateLoop();

  private:
	Events event = Events::NONE;
	rtos::EventFlags event_flags {};

	std::variant<interface::State, WaitingForCommands, Sleep> _state = WaitingForCommands();
};

}	// namespace leka

#endif	 // _LEKA_OS_STATE_MACHINE_
