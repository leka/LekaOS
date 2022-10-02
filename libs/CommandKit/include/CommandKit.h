// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

#include "CircularQueue.h"
#include "interface/Command.h"
#include "interface/libs/EventLoop.h"

namespace leka {

class CommandKit
{
  public:
	explicit CommandKit(interface::EventLoop &loop);

	void push(std::span<uint8_t> data);

	void registerCommand(std::span<interface::Command *> cmds);

	[[nodiscard]] auto size() const -> std::size_t;

  private:
	void processCommands();
	void executeCommands();

	static constexpr auto kStartPattern = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A});

	CircularQueue<uint8_t, 1024> _input_buffer {};
	CircularQueue<uint8_t, 1024> _command_buffer {};
	std::array<uint8_t, 128> _tmp_buffer {};

	std::span<interface::Command *> _commands {};

	interface::EventLoop &_event_loop;
};

}	// namespace leka
