// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "Command.h"
#include "LogKit.h"
#include "Utils.h"

namespace leka {

struct LedCommand : interface::Command {
	auto id() -> uint8_t override { return cmd::id; }
	auto data() -> uint8_t * override { return args.data(); };
	[[nodiscard]] auto size() const -> std::size_t override { return std::size(args); };

	void execute() override
	{
		auto [pos, id, r, g, b, chcksm] = std::tuple_cat(args);

		if (chcksm != utils::math::checksum8(std::span {args.data(), args.size() - 1})) {
			log_error("wrong checksum");
			return;
		}

		switch (pos) {
			case cmd::position::ears: {
				log_debug("turn on ear id %i with %i / %i / %i", id, r, g, b);
				break;
			}
			case cmd::position::belt: {
				log_debug("turn on belt id %i with %i / %i / %i", id, r, g, b);
				break;
			}
			default:
				log_error("pos not found");
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
};

}	// namespace leka
