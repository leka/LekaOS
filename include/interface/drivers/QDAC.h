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

	virtual void init()													   = 0;
	virtual void write(uint8_t channel, uint16_t data, bool b_eep = false) = 0;
	virtual auto read(uint8_t channel, bool b_eep = false) -> uint16_t	   = 0;
};

}	// namespace leka::interface
