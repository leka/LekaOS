// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLED.h"

#include "CoreSPI.h"
#include "gtest/gtest.h"
#include "mocks/leka/SPI.h"

using namespace leka;

TEST(CoreLedTest, initialization)
{
	auto spi  = CoreSPI {NC, NC, NC, NC};
	auto leds = CoreLED<20> {spi};

	ASSERT_NE(&leds, nullptr);
}
