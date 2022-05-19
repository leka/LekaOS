// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACExpander.h"

#include "CoreI2C.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "span"

using namespace leka;
using ::testing::_;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

class CoreDACExpanderTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	const uint8_t i2c_address {0x4E};
	mock::CoreI2C mocki2c;
	mbed::DigitalOut expander_reset {PinName::SENSOR_PROXIMITY_MUX_RESET, 0};
	CoreDACExpanderMCP4728 expander {mocki2c, expander_reset};
};
