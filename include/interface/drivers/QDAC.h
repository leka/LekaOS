// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>
#include <span>

namespace leka::interface {
class QDAC
{
  public:
	virtual ~QDAC() = default;

	virtual void init()								   = 0;
	virtual void write(uint8_t channel, uint16_t data) = 0;
	virtual auto read(uint8_t channel) -> uint16_t	   = 0;
};

}	// namespace leka::interface
