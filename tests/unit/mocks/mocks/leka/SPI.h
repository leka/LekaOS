// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "SPI.h"
#include "gmock/gmock.h"

namespace leka::mock {

class SPI : public interface::SPI
{
  public:
	MOCK_METHOD(size_t, write, (std::span<const uint8_t>), (override));
};

}	// namespace leka::mock
