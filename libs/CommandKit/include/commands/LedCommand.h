// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "LogKit.h"
#include "RGB.h"
#include "Utils.h"
#include "interface/Command.h"
#include "interface/drivers/LED.h"

namespace leka {

struct LedCommand : interface::Command {
	LedCommand(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {}

	auto id() -> uint8_t override { return cmd::id; }

	auto data() -> uint8_t * override
	{
		args = {};
		return args.data();
	};

	[[nodiscard]] auto size() const -> std::size_t override { return std::size(args); };

	void execute() override
	{
		auto [pos, id, r, g, b, chcksm] = std::tuple_cat(args);

		auto expected = utils::math::checksum8(std::span {args.data(), args.size() - 1});
		if (chcksm != expected) {
			return;
		}

		switch (pos) {
			case cmd::position::ears: {
				_ears.setColorAtIndex(id, RGB {r, g, b});
				_ears.show();
				break;
			}
			case cmd::position::belt: {
				_belt.setColorAtIndex(id, RGB {r, g, b});
				_belt.show();
				break;
			}
			default:
				break;
		}
	}

  private:
	struct cmd {
		static constexpr auto id   = uint8_t {0x10};
		static constexpr auto size = uint8_t {1 + 1 + 3 + 1};	// EAR/BELT + Id + R, G, B + Checksum

		struct position {
			static constexpr auto ears = uint8_t {0x11};
			static constexpr auto belt = uint8_t {0x12};
		};
	};

	std::array<uint8_t, cmd::size> args {};
	interface::LED &_ears;
	interface::LED &_belt;
};

}	// namespace leka
