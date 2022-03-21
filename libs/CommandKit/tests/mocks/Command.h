// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "LogKit.h"
#include "Utils.h"
#include "commands/TestCommand.h"
#include "gmock/gmock.h"
#include "interface/Command.h"

namespace leka::mock {

struct Command : TestCommand {
	Command(uint8_t id) : _cmd(cmd {id}) {}

	auto id() -> uint8_t override { return _cmd.id; }

	auto execute() -> bool override
	{
		if (!TestCommand::execute()) {
			return false;
		}

		HasBeenCalled();   // ? Call Mock method
		return false;
	}

	MOCK_METHOD(void, HasBeenCalled, (), ());

  private:
	struct cmd {
		uint8_t id;
		static constexpr auto size = uint8_t {1 + 1 + 3 + 1};	// p1 (ONE/TWO) + p2 + d{1, 2, 3} + Checksum

		struct param {
			static constexpr auto one = uint8_t {0x11};
			static constexpr auto two = uint8_t {0x12};
		};
	};

	cmd _cmd;
};

}	// namespace leka::mock
