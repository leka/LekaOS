// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "LogKit.h"
#include "Utils.h"
#include "gmock/gmock.h"
#include "interface/Command.h"

namespace leka::mock {

struct Command : interface::Command {
	Command(uint8_t id) : _cmd(cmd {id}) {}

	auto id() -> uint8_t override { return _cmd.id; }

	auto data() -> uint8_t * override
	{
		args = {};
		return args.data();
	};

	[[nodiscard]] auto size() const -> std::size_t override { return std::size(args); };

	void execute() override
	{
		auto [param1, param2, data1, data2, data3, chcksm] = std::tuple_cat(args);

		auto expected = [&] { return utils::math::checksum8(std::span {args.data(), args.size() - 1}); };

		if (chcksm != expected()) {
			log_error("wrong checksum, expected: 0x%02hX", expected());
			return;
		}

		switch (param1) {
			case cmd::param::one:
				log_debug("TestCommand cmd::param::one p2: %i - d1: %i, d2: %i, d3: %i", param2, data1, data2, data3);
				break;

			case cmd::param::two:
				log_debug("TestCommand cmd::param::two p2: %i - d1: %i, d2: %i, d3: %i", param2, data1, data2, data3);
				break;

			default:
				break;
		}

		HasBeenCalled();   // ? Call Mock method
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
	std::array<uint8_t, cmd::size> args {};
};

}	// namespace leka::mock
