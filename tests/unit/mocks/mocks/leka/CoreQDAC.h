// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreBufferedSerial.h"
#include "gmock/gmock.h"

namespace leka {

class CoreQDAC : public interface::QDAC
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, write, (uint8_t, std::array<uint8_t, 2>, uint8_t), (override));
	MOCK_METHOD(void, read, (std::span<uint8_t>, size_t), (override));
};

}	// namespace leka
