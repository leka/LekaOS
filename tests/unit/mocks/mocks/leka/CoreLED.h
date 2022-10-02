// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "gmock/gmock.h"
#include "interface/drivers/LED.h"

namespace leka::mock {

class CoreLED : public interface::LED
{
  public:
	MOCK_METHOD(void, setColor, (const RGB &color), (override));

	MOCK_METHOD(void, setColorRange, (unsigned start, unsigned end, const RGB &color), (override));
	MOCK_METHOD(void, setColorAtIndex, (unsigned index, const RGB &color), (override));
	MOCK_METHOD(void, setColorWithArray, (std::span<const RGB> color), (override));

	MOCK_METHOD(void, fadeToBlackBy, (const uint8_t &value), (override));

	MOCK_METHOD(void, show, (), (override));
	MOCK_METHOD(void, hide, (), (override));

	MOCK_METHOD(std::span<const RGB>, getColor, (), (override));
	MOCK_METHOD(std::span<const uint8_t>, getBrightness, (), (override));

	MOCK_METHOD(bool, isOn, (), (const, override));
};

}	// namespace leka::mock
