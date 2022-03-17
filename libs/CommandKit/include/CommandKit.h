// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

#include "CircularQueue.h"
#include "commands/LedCommand.h"
#include "interface/Command.h"

namespace leka {

class CommandKit
{
  public:
	CommandKit();

	void push(std::span<uint8_t> data);

	void registerCommand(std::span<interface::Command *> cmds);

	auto size() const -> std::size_t;

  private:
	void processCommands();
	void executeCommands();

	CircularQueue<uint8_t, 256> _input_buffer {};

	std::span<interface::Command *> _commands {};
};

}	// namespace leka
