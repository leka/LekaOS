// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "BehaviorKit.h"
#include "Utils.h"
#include "interface/Command.h"

namespace leka {

struct ReinforcerCommand : interface::Command {
	explicit ReinforcerCommand(BehaviorKit &kit) : _behaviorkit(kit) {}

	auto id() -> uint8_t override { return cmd::id; }

	auto data() -> uint8_t * override
	{
		args = {};
		return args.data();
	}

	[[nodiscard]] auto size() const -> std::size_t override { return std::size(args); }

	auto execute() -> bool override
	{
		auto [id, chcksm] = std::tuple_cat(args);

		auto expected = [&] { return utils::math::checksum8(std::span {args.data(), args.size() - 1}); };

		if (chcksm != expected()) {
			return false;
		}

		switch (id) {
			case cmd::motivator::rainbow:
				_behaviorkit.rainbow();
				break;
			case cmd::motivator::fire:
				_behaviorkit.fire();
				break;
			case cmd::motivator::sprinkles:
				_behaviorkit.sprinkles();
				break;
			case cmd::motivator::spinblink:
				_behaviorkit.spinBlink();
				break;
			case cmd::motivator::blinkgreen:
				_behaviorkit.blinkGreen();
				break;
			default:
				_behaviorkit.rainbow();
				break;
		}

		return true;
	}

  private:
	struct cmd {
		static constexpr auto id   = uint8_t {0x50};
		static constexpr auto size = uint8_t {1 + 1};	// id + Checksum

		struct motivator {
			static constexpr auto rainbow	 = uint8_t {0x51};
			static constexpr auto fire		 = uint8_t {0x52};
			static constexpr auto sprinkles	 = uint8_t {0x53};
			static constexpr auto spinblink	 = uint8_t {0x54};
			static constexpr auto blinkgreen = uint8_t {0x55};
		};
	};

	std::array<uint8_t, cmd::size> args {};
	BehaviorKit &_behaviorkit;
};

}	// namespace leka
