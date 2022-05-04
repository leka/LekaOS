// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

#include "CircularQueue.h"
#include "CoreEventQueue.h"
#include "interface/Command.h"

namespace leka {

class CommandKit
{
  public:
	CommandKit();

	void push(std::span<uint8_t> data);

	void registerCommand(std::span<interface::Command *> cmds);

	[[nodiscard]] auto size() const -> std::size_t;

  private:
	void processCommands();
	void executeCommands();

	static constexpr auto kStartPattern = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A});

	// CoreEventQueue _event_queue {};
	rtos::Thread _event_queue_thread {};
	events::EventQueue _event_queue {};

	CircularQueue<uint8_t, 1024> _input_buffer {};

	std::span<interface::Command *> _commands {};

	bool _is_ready_to_process = true;
};

}	// namespace leka
