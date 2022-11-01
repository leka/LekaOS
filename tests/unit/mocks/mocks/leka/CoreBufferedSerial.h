// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/BufferedSerial.h"

namespace leka::mock {

class CoreBufferedSerial : public interface::BufferedSerial
{
  public:
	MOCK_METHOD(std::size_t, read, (uint8_t *, std::size_t), (override));
	MOCK_METHOD(std::size_t, write, (const uint8_t *, std::size_t), (override));
	MOCK_METHOD(bool, readable, (), (override));
	MOCK_METHOD(void, disable_input, (), (override));
	MOCK_METHOD(void, enable_input, (), (override));
	MOCK_METHOD(void, sigio, (mbed::Callback<void()>), (override));
};

}	// namespace leka::mock
