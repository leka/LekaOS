// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "Utils.h"
#include "interface/Command.h"
#include "interface/libs/VideoKit.h"

namespace leka {

struct VideoCommand : interface::Command {
	explicit VideoCommand(interface::VideoKit &kit) : _videokit(kit) {}

	auto id() -> uint8_t override { return cmd::id; }

	auto data() -> uint8_t * override
	{
		args = {};
		return args.data();
	}

	[[nodiscard]] auto size() const -> std::size_t override { return std::size(args); }

	auto execute() -> bool override
	{
		auto [id_high, id_low, chcksm] = std::tuple_cat(args);

		auto expected = [&] { return utils::math::checksum8(std::span {args.data(), args.size() - 1}); };

		if (chcksm != expected()) {
			return false;
		}

		auto id							= (id_high << 8) | id_low;
		std::array<char, 32> image_path = {};
		snprintf(image_path.data(), image_path.size(), "/fs/home/img/id/%.4X.jpg", id);

		_videokit.fillWhiteBackgroundAndDisplayImage(std::filesystem::path {image_path.data()});

		return true;
	}

  private:
	struct cmd {
		static constexpr auto id   = uint8_t {0x80};
		static constexpr auto size = uint8_t {2 + 1};	// image_id + Checksum
	};

	std::array<uint8_t, cmd::size> args {};
	interface::VideoKit &_videokit;
};

}	// namespace leka
