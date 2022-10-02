// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "Utils.h"
#include "interface/Command.h"
#include "interface/drivers/Motor.h"

namespace leka {

struct MotorsCommand : interface::Command {
	MotorsCommand(interface::Motor &left, interface::Motor &right) : _left(left), _right(right) {}

	auto id() -> uint8_t override { return cmd::id; }

	auto data() -> uint8_t * override
	{
		args = {};
		return args.data();
	}

	[[nodiscard]] auto size() const -> std::size_t override { return std::size(args); }

	auto execute() -> bool override
	{
		auto [motor, dir, speed, chcksm] = std::tuple_cat(args);

		auto expected = [&] { return utils::math::checksum8(std::span {args.data(), args.size() - 1}); };

		if (chcksm != expected()) {
			return false;
		}

		static constexpr auto kInt2Float = float {255.F};

		auto spd	   = float {static_cast<float>(speed) / kInt2Float};
		auto right_dir = dir == 1 ? 0 : 1;

		switch (motor) {
			case cmd::motor::left:
				_left.spin(static_cast<rotation_t>(dir), spd);
				break;

			case cmd::motor::right:

				_right.spin(static_cast<rotation_t>(right_dir), spd);
				break;

			default:
				break;
		}

		return true;
	}

  private:
	struct cmd {
		static constexpr auto id   = uint8_t {0x20};
		static constexpr auto size = uint8_t {1 + 2 + 1};	// id + {direction,speed} + checksum

		struct motor {
			static constexpr auto left	= uint8_t {0x21};
			static constexpr auto right = uint8_t {0x22};
		};
	};

	std::array<uint8_t, cmd::size> args {};
	interface::Motor &_left;
	interface::Motor &_right;
};

}	// namespace leka
