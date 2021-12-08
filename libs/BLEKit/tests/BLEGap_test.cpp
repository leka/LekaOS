// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEGap.h"

#include "ble_mocks.h"

#include "gtest/gtest.h"

using namespace leka;

class BLEGapTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		BLE &ble = BLE::Instance();
		// BLEGap ble_gap(ble, event_queue);
	}
	void TearDown() override { ble::delete_mocks(); }

	EventQueue event_queue;
};

TEST_F(BLEGapTest, initialization) {}
