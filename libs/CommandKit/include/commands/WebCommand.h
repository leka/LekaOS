// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

namespace leka {

struct WebCommand : interface::Command {
	explicit WebCommand() {}

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
			default:
				break;
		}

		return true;
	}

  private:
	struct cmd {
		static constexpr auto id   = uint8_t {0xA0};
		static constexpr auto size = uint8_t {1 + n + 1};	// id + data + Checksum

		struct motivator {
			static constexpr auto setSSID	 			= uint8_t {0xA1}; // 1+n bytes
			static constexpr auto setPassword			= uint8_t {0xA2}; // 1+n bytes
			static constexpr auto sendCredentials		= uint8_t {0xA3}; // 1+n bytes
			static constexpr auto nameCredentials	 	= uint8_t {0xA4}; // 1+n bytes
			static constexpr auto setCredentialsUsed	= uint8_t {0xA5}; // 1+n bytes
			static constexpr auto connect				= uint8_t {0xA6}; // 1+0 byte
			static constexpr auto URLToDownload			= uint8_t {0xA7}; // 1+n bytes
			static constexpr auto fileName				= uint8_t {0xA8}; // 1+n bytes
			static constexpr auto download				= uint8_t {0xA9}; // 1+0 byte
		};
	};

	std::array<uint8_t, cmd::size> args {};
};

}	// namespace leka
