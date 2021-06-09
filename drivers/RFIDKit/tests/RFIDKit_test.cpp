// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"

#include "gtest/gtest.h"

using namespace leka;
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreRFID.h"

using namespace leka;
using namespace interface;

class CoreRFIDKitTest : public ::testing::Test
{
  protected:
	CoreRFIDKitTest() : coreRfid(mockRfid) {};

	// void SetUp() override {}
	// void TearDown() override {}

	RFIDKit coreRfid;
	CoreRFIDMock mockRfid;
};

TEST_F(CoreRFIDKitTest, initialization)
{
	ASSERT_NE(&coreRfid, nullptr);
}
