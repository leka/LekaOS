// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <span>

#include "array"

namespace leka::interface {

class QDAC
{
  public:
	virtual ~QDAC() = default;

	virtual void init()												 = 0;
	virtual void write(uint8_t channel, std::array<uint8_t, 2> data) = 0;
	virtual void read(std::span<uint8_t> buffer)					 = 0;
};

}	// namespace leka::interface
