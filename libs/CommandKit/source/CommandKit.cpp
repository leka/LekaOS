// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CommandKit.h"

#include "Utils.h"

using namespace leka;

CommandKit::CommandKit()
{
	// _event_queue.dispatch_forever();
	_event_queue_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
}

void CommandKit::push(std::span<uint8_t> data)
{
	_input_buffer.push(data);

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
	_is_ready_to_process = false;

	auto pos = 0U;

	if (!_input_buffer.hasPattern(kStartPattern, pos)) {
		_input_buffer.clear();
		_is_ready_to_process = true;
		return;
	}

	for (auto i = 0U; i < pos + kStartPattern.size(); ++i) {
		_input_buffer.drop();
	}

	executeCommands();
}

void CommandKit::executeCommands()
{
	auto number_of_commands = uint8_t {};
	_input_buffer.pop(number_of_commands);

	for (auto i = 0; i < number_of_commands; ++i) {
		auto current_command = uint8_t {};
		_input_buffer.pop(current_command);

		for (auto *cmd: _commands) {
			if (current_command == cmd->id()) {
				_input_buffer.pop(cmd->data(), cmd->size());
				auto exec = [cmd, current_command] {
					auto *lcmd = cmd;
					log_debug("Execute command: %i", current_command);
					lcmd->execute();
				};
				// _event_queue.call(exec);
				_event_queue.call(cmd, &interface::Command::execute);
				// exec();
			}
		}
	}

	_is_ready_to_process = true;
}
