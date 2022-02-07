// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEKit.h"

#include "ble_mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/leka/CoreEventQueue.h"
#include "stubs/mbed/BLE.h"

using namespace leka;
using namespace ble;

using ::testing::AnyNumber;
using ::testing::Return;

class BLEKitTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		ble::init_mocks();
		spy_ble_reset();
	}
	void TearDown() override { ble::delete_mocks(); }

	BLEKit ble {};
	GapMock &mock_gap = gap_mock();

	void expectStartAdvertisingCall()
	{
		EXPECT_CALL(mock_gap, isAdvertisingActive).WillOnce(Return(false));
		EXPECT_CALL(mock_gap, startAdvertising).Times(1);
		EXPECT_CALL(mock_gap, setAdvertisingParameters).Times(1);
		EXPECT_CALL(mock_gap, setAdvertisingPayload).Times(1);
	};
};

TEST_F(BLEKitTest, initialization)
{
	EXPECT_NE(&ble, nullptr);
}

TEST_F(BLEKitTest, init)
{
	spy_ble_hasInitialized_return_value = false;

	EXPECT_CALL(mock_gap, setEventHandler).Times(1);
	expectStartAdvertisingCall();

	ble.init();

	BLE::InitializationCompleteCallbackContext context = {BLE::Instance(), BLE_ERROR_NONE};
	spy_ble_on_initialization_complete_callback(&context);

	EXPECT_TRUE(spy_ble_did_call_initialization);
}

TEST_F(BLEKitTest, initBLEAlreadyInitialized)
{
	spy_ble_hasInitialized_return_value = true;

	ble.init();

	BLE::InitializationCompleteCallbackContext context = {BLE::Instance(), BLE_ERROR_NONE};
	spy_ble_on_initialization_complete_callback(&context);

	EXPECT_FALSE(spy_ble_did_call_initialization);
}

TEST_F(BLEKitTest, callOnEventsToProcess)
{
	spy_ble_hasInitialized_return_value	 = false;
	spy_CoreEventQueue_did_call_function = false;

	EXPECT_CALL(mock_gap, setEventHandler).Times(AnyNumber());

	ble.init();

	EXPECT_FALSE(spy_CoreEventQueue_did_call_function);

	BLE::OnEventsToProcessCallbackContext context = {BLE::Instance()};
	spy_ble_on_events_to_process_callback(&context);

	EXPECT_TRUE(spy_CoreEventQueue_did_call_function);
}
