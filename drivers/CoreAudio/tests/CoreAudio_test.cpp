// // Leka - LekaOS
// // Copyright 2021 APF France handicap
// // SPDX-License-Identifier: Apache-2.0

#include "CoreAudio.h"

#include "gtest/gtest.h"
#include "mocks/leka/CoreDAC.h"
#include "mocks/leka/CoreDACTimer.h"
#include "mocks/leka/LKCoreSTM32Hal.h"

using namespace leka;
using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;

class CoreAudioTest : public ::testing::Test
{
  protected:
	CoreAudioTest() : coreaudio(halmock, dacmock, timermock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	LKCoreSTM32HalMock halmock;
	CoreDACMock dacmock;
	CoreDACTimerMock timermock;

	CoreAudio coreaudio;
};

TEST_F(CoreAudioTest, instantiation)
{
	ASSERT_NE(&coreaudio, nullptr);
};