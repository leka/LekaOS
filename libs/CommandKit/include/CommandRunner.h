// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

#include "CircularQueue.h"
#include "LogKit.h"
#include "ThisThread.h"
#include "interface/Command.h"
#include "interface/libs/EventLoop.h"

using namespace std::chrono;

namespace leka {

class CommandRunner
{
  public:
	CommandRunner(interface::EventLoop &loop) : _loop(loop)
	{
		_loop.registerCallback([this] { executeCommands(); });
	}

	void registerCommand(std::span<interface::Command *> cmds) { _commands = cmds; }

	void push(std::span<uint8_t> data)
	{
		_input_buffer.push(data);
		_loop.start();
	}

	void executeCommands()
	{
		while (_input_buffer.size() != 0) {
			auto current_command = uint8_t {};
			_input_buffer.pop(current_command);

			for (auto *cmd: _commands) {
				if (current_command == cmd->id()) {
					_input_buffer.pop(cmd->data(), cmd->size());
					cmd->execute();
				}
			}
		}
		// _loop.stop();
	}

  private:
	interface::EventLoop &_loop;
	CircularQueue<uint8_t, 1024> _input_buffer {};
	std::span<interface::Command *> _commands {};
};

}	// namespace leka
