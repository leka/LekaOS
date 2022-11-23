// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEKit.h"
#include <array>

#include "ble_mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/BLEService.h"
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
	GapMock &mbed_mock_gap		   = gap_mock();
	GattServerMock &mbed_mock_gatt = gatt_server_mock();

	void expectStartAdvertisingCall()
	{
		EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).WillOnce(Return(false));
		EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(1);
		EXPECT_CALL(mbed_mock_gap, setAdvertisingParameters).Times(1);
		EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(1);
	};
};

TEST_F(BLEKitTest, initialization)
{
	EXPECT_NE(&ble, nullptr);
}

TEST_F(BLEKitTest, init)
{
	spy_ble_hasInitialized_return_value = false;

	EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(1);
	EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(1);
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

TEST_F(BLEKitTest, setServices)
{
	auto characteristic_value		  = uint8_t {};
	auto characteristic				  = GattCharacteristic {0x1234, &characteristic_value};
	auto service_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});

	auto mock_service_1 = mock::BLEService(0x01, service_characteristic_table);
	auto mock_service_2 = mock::BLEService(0x02, service_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service_1, &mock_service_2});

	EXPECT_CALL(mbed_mock_gatt, addService).Times(std::size(services));

	ble.setServices(services);
}

TEST_F(BLEKitTest, callOnEventsToProcess)
{
	spy_ble_hasInitialized_return_value	 = false;
	spy_CoreEventQueue_did_call_function = false;

	EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(AnyNumber());
	EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(AnyNumber());

	ble.init();

	EXPECT_FALSE(spy_CoreEventQueue_did_call_function);

	BLE::OnEventsToProcessCallbackContext context = {BLE::Instance()};
	spy_ble_on_events_to_process_callback(&context);

	EXPECT_TRUE(spy_CoreEventQueue_did_call_function);
}

TEST_F(BLEKitTest, getAdvertisingDataThenSetAdvertisingData)
{
	auto advertising_data = ble.getAdvertisingData();
	advertising_data.name = "NewLeka";

	EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(1);

	ble.setAdvertisingData(advertising_data);
}

TEST_F(BLEKitTest, onConnectionCallback)
{
	auto dummy_on_connection_callback = std::function<void()> {};

	ble.onConnectionCallback(dummy_on_connection_callback);

	// nothing expected
}

TEST_F(BLEKitTest, onDisconnectionCallback)
{
	auto dummy_on_disconnection_callback = std::function<void()> {};

	ble.onDisconnectionCallback(dummy_on_disconnection_callback);

	// nothing expected
}
