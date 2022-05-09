// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

#include "CircularQueue.h"
#include "commands/LedFullCommand.h"
#include "commands/LedRangeCommand.h"
#include "commands/LedSingleCommand.h"
#include "commands/MotorsCommand.h"
#include "commands/ReinforcerCommand.h"
#include "commands/TestCommand.h"
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

	CircularQueue<uint8_t, 256> _input_buffer {};

	std::span<interface::Command *> _commands {};

	bool _is_ready_to_process = true;
};

}	// namespace leka
