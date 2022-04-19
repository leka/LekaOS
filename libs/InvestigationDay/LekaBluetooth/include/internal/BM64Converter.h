// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>
#include <cstdint>

namespace BM64 {
namespace Command {

	const uint8_t pairing[3]			   = {0x02, 0x00, 0x50};
	inline constexpr size_t pairing_length = 3;

	const uint8_t play_pause[3]				  = {0x02, 0x00, 0x32};
	inline constexpr size_t play_pause_length = 3;

	const uint8_t eq_soft_mode[3]				= {0x1C, 0x01, 0x00};
	inline constexpr size_t eq_soft_mode_length = 3;

};	 // namespace Command
// using command_t = Command;

namespace Response {

	const uint8_t acknowledge[2]			   = {0xFF, 0xFF};	 // Not done yet
	inline constexpr size_t acknowledge_length = 2;

};	 // namespace Response
// using response_t = Response;

size_t getCommand(const uint8_t *cmd, const size_t cmd_length, uint8_t *buffer);
// void convertResponse();	  // Events in datasheet

};	 // namespace BM64
