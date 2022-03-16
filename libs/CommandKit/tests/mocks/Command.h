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
	auto data() -> uint8_t * override { return args.data(); };
	[[nodiscard]] auto size() const -> std::size_t override { return std::size(args); };

	void execute() override
	{
		auto [pos, id, r, g, b, chcksm] = std::tuple_cat(args);

		if (chcksm != utils::math::checksum8(std::span {args.data(), args.size() - 1})) {
			log_error("wrong checksum, got 0x%02hX, should be 0x%02hX", chcksm,
					  utils::math::checksum8(std::span {args.data(), args.size() - 1}));
			return;
		}

		switch (pos) {
			case cmd::position::ears: {
				log_debug("turn on ear id 0x%02hX with 0x%02hX / 0x%02hX / 0x%02hX", id, r, g, b);
				break;
			}
			case cmd::position::belt: {
				log_debug("turn on belt id 0x%02hX with 0x%02hX / 0x%02hX / 0x%02hX", id, r, g, b);
				break;
			}
			default:
				log_error("pos not found");
				return;
		}

		HasBeenCalled();   // ? Call Mock method
	}

	MOCK_METHOD(void, HasBeenCalled, (), ());

	struct cmd {
		uint8_t id;
		static constexpr auto size = uint8_t {1 + 1 + 3 + 1};	// EAR/BELT + Id + R, G, B + Checksum

		struct position {
			static constexpr auto ears = uint8_t {0x11};
			static constexpr auto belt = uint8_t {0x12};
		};
	};

  private:
	cmd _cmd;
	std::array<uint8_t, cmd::size> args {};
};

}	// namespace leka::mock
