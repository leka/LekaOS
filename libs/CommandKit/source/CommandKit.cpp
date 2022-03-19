// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CommandKit.h"

#include "LogKit.h"
#include "Utils.h"

using namespace leka;

CommandKit::CommandKit() = default;

void CommandKit::push(std::span<uint8_t> data)
{
	_input_buffer.push(data.data(), data.size());

	constexpr auto kMinCommandSize = 7;
	if (_input_buffer.size() >= kMinCommandSize && _is_ready_to_process) {
		processCommands();
	}
}

void CommandKit::registerCommand(std::span<interface::Command *> cmds)
{
	_commands = cmds;
}

auto CommandKit::size() const -> std::size_t
{
	return _commands.size();
}

void CommandKit::processCommands()
{
	static auto kStartPattern = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A});

	_is_ready_to_process = false;

	auto pos = int {};

	if (!_input_buffer.hasPattern(kStartPattern.data(), kStartPattern.size(), pos)) {
		log_debug("start fram NOT found, resetting");
		_input_buffer.reset();
		_is_ready_to_process = true;
		return;
	}

	log_debug("start frame found at %i", pos);

	for (auto i = 0; i < pos + kStartPattern.size(); ++i) {
		auto _ = uint8_t {};
		_input_buffer.pop(_);
	}

	executeCommands();
}

void CommandKit::executeCommands()
{
	auto number_of_commands = uint8_t {};
	_input_buffer.pop(number_of_commands);
	log_debug("number of commands %i", number_of_commands);

	for (auto i = 0; i < number_of_commands; ++i) {
		auto current_command = uint8_t {};
		_input_buffer.pop(current_command);

		for (auto *cmd: _commands) {
			if (current_command == cmd->id()) {
				_input_buffer.pop(cmd->data(), cmd->size());
				cmd->execute();
			}
		}
	}

	_is_ready_to_process = true;
}
