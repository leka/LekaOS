// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace BM64 {
namespace Command {

	inline constexpr uint8_t SINC_WORD = 0xAA;

	inline constexpr uint8_t SEND_DATA	   = 0x12;
	inline constexpr uint8_t DATA_INCOMING = 0x22;

	inline constexpr uint8_t BTM_STATUS			   = 0x01;
	inline constexpr uint8_t A2DP_LINK_ESTABLISHED = 0x06;

};	 // namespace Command

namespace ComplexCommand {

	inline static auto pairing					= std::to_array<uint8_t>({0x02, 0x00, 0x50});
	inline constexpr std::size_t pairing_length = 3;

	inline static auto play_pause				   = std::to_array<uint8_t>({0x02, 0x00, 0x32});
	inline constexpr std::size_t play_pause_length = 3;

	inline static auto eq_soft_mode					 = std::to_array<uint8_t>({0x1C, 0x01, 0x00});
	inline constexpr std::size_t eq_soft_mode_length = 3;

}	// namespace ComplexCommand

// using command_t = Command;

namespace Response {

	inline static auto acknowledge					= std::to_array<uint8_t>({0xFF, 0xFF});	  // Not done yet
	inline constexpr std::size_t acknowledge_length = 2;

};	 // namespace Response
// using response_t = Response;

auto getCommand(std::span<uint8_t> cmd, std::size_t cmd_length, std::span<uint8_t> buffer) -> std::size_t;
// void convertResponse();	  // Events in datasheet

};	 // namespace BM64
