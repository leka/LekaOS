// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

#include "CircularQueue.h"
#include "Command.h"
#include "commands/LedCommand.h"

namespace leka {

class CommandKit
{
  public:
	CommandKit();

	void push(std::span<uint8_t> data);
	void registerCommand(std::span<Command *> cmds);

	struct command {
		static inline auto led = LedCommand {};
	};

	auto size() const -> std::size_t;

  private:
	void processCommands();
	void executeCommands();

	CircularQueue<uint8_t, 256> _input_buffer {};

	std::span<Command *> _commands {};
};

}	// namespace leka
