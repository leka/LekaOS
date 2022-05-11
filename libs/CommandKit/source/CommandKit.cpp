// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CommandKit.h"

#include "Utils.h"

using namespace leka;

CommandKit::CommandKit(interface::EventLoop &loop) : _event_loop(loop)
{
	_event_loop.registerCallback([this] { executeCommands(); });
}

void CommandKit::push(std::span<uint8_t> data)
{
	_input_buffer.push(data);

	static constexpr auto kMinCommandSize = 7;

	if (_input_buffer.size() >= kMinCommandSize) {
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
	auto pos = 0U;

	if (!_input_buffer.hasPattern(kStartPattern, pos)) {
		_input_buffer.clear();
		return;
	}

	for (auto i = 0U; i < pos + kStartPattern.size(); ++i) {
		_input_buffer.drop();
	}

	auto number_of_commands = uint8_t {};
	_input_buffer.pop(number_of_commands);

	for (auto i = 0; i < number_of_commands; ++i) {
		auto current_command = uint8_t {};
		_input_buffer.pop(current_command);

		for (auto *cmd: _commands) {
			if (current_command == cmd->id()) {
				_tmp_buffer.at(0) = current_command;
				_input_buffer.pop(&_tmp_buffer[1], cmd->size());
				_command_buffer.push({_tmp_buffer.data(), cmd->size() + 1});
			}
		}
	}

	_event_loop.start();
}

void CommandKit::executeCommands()
{
	while (!_command_buffer.empty()) {
		auto current_command = uint8_t {};
		_command_buffer.pop(current_command);

		for (auto *cmd: _commands) {
			if (current_command == cmd->id()) {
				_command_buffer.pop(cmd->data(), cmd->size());
				cmd->execute();
			}
		}
	}

	_event_loop.stop();
}
