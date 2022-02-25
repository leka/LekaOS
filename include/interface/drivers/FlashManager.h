// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

namespace leka::interface {

class FlashManager
{
  public:
	virtual ~FlashManager() = default;

	virtual auto getStatusRegister() -> uint8_t = 0;

	virtual void waitForChipAvailable()		  = 0;
	virtual auto chipIsNotAvailable() -> bool = 0;

	virtual void enableWrite()				 = 0;
	virtual auto writeIsNotEnabled() -> bool = 0;

	virtual void reset() = 0;
	virtual void erase() = 0;
};
}	// namespace leka::interface
