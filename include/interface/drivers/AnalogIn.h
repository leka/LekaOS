// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka::interface {

class AnalogIn
{
  public:
	virtual ~AnalogIn() = default;

	virtual auto read() -> float = 0;
};

}	// namespace leka::interface
