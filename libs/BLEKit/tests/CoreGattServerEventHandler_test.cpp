// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGattServerEventHandler.h"

#include "ble_mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/BLEService.h"

using namespace leka;
using namespace ble;

using ::testing::InSequence;
using ::testing::MockFunction;
using ::testing::Return;

class CoreGattServerEventHandlerTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	void TearDown() override { ble::delete_mocks(); }

	BLE &ble = BLE::Instance();
	CoreGattServerEventHandler gatt_event_handler {};
};

MATCHER_P(compareParams, expected_params, "")
{
	bool same_handle = expected_params.handle == arg.handle;

	return same_handle;
}

TEST_F(CoreGattServerEventHandlerTest, initialization)
{
	EXPECT_NE(&gatt_event_handler, nullptr);
}

TEST_F(CoreGattServerEventHandlerTest, onDataReceived)
{
	auto characteristic_value		  = uint8_t {0};
	auto characteristic				  = GattCharacteristic {0x1234, &characteristic_value};
	auto service_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});
	auto mock_service				  = mock::BLEService(0x01, service_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service});

	gatt_event_handler.setServices(services);

	auto expected_params   = GattWriteCallbackParams {};
	expected_params.handle = characteristic.getValueHandle();

	EXPECT_CALL(mock_service, onDataReceived(compareParams(expected_params)))
		.Times(std::size(service_characteristic_table));

	gatt_event_handler.onDataWritten(expected_params);
}

TEST_F(CoreGattServerEventHandlerTest, onDataReceivedMultipleServices)
{
	auto characteristic_value			= uint8_t {};
	auto characteristic					= GattCharacteristic {0x1234, &characteristic_value};
	auto service_1_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});
	auto service_2_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic, &characteristic});
	auto mock_service_1					= mock::BLEService(0x01, service_1_characteristic_table);
	auto mock_service_2					= mock::BLEService(0x02, service_2_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service_1, &mock_service_2});

	gatt_event_handler.setServices(services);

	auto expected_params   = GattWriteCallbackParams {};
	expected_params.handle = characteristic.getValueHandle();

	EXPECT_CALL(mock_service_1, onDataReceived(compareParams(expected_params)))
		.Times(std::size(service_1_characteristic_table));
	EXPECT_CALL(mock_service_2, onDataReceived(compareParams(expected_params)))
		.Times(std::size(service_2_characteristic_table));

	gatt_event_handler.onDataWritten(expected_params);
}

TEST_F(CoreGattServerEventHandlerTest, onDataReceivedParamsHandleDifferent)
{
	auto characteristic_value		  = uint8_t {};
	auto characteristic				  = GattCharacteristic {0x1234, &characteristic_value};
	auto service_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});
	auto mock_service				  = mock::BLEService(0x01, service_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service});

	gatt_event_handler.setServices(services);

	auto params	  = GattWriteCallbackParams {};
	params.handle = characteristic.getValueHandle() + 1;

	EXPECT_CALL(mock_service, onDataReceived).Times(0);

	gatt_event_handler.onDataWritten(params);
}

TEST_F(CoreGattServerEventHandlerTest, onDataRequested)
{
	auto characteristic_value		  = uint8_t {0};
	auto characteristic				  = GattCharacteristic {0x1234, &characteristic_value};
	auto service_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});
	auto mock_service				  = mock::BLEService(0x01, service_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service});

	gatt_event_handler.setServices(services);

	auto expected_params   = GattReadCallbackParams {};
	expected_params.handle = characteristic.getValueHandle();

	EXPECT_CALL(mock_service, onDataRequested(compareParams(expected_params)))
		.Times(std::size(service_characteristic_table));

	gatt_event_handler.onDataRead(expected_params);
}

TEST_F(CoreGattServerEventHandlerTest, onDataRequestedMultipleServices)
{
	auto characteristic_value			= uint8_t {};
	auto characteristic					= GattCharacteristic {0x1234, &characteristic_value};
	auto service_1_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});
	auto service_2_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic, &characteristic});
	auto mock_service_1					= mock::BLEService(0x01, service_1_characteristic_table);
	auto mock_service_2					= mock::BLEService(0x02, service_2_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service_1, &mock_service_2});

	gatt_event_handler.setServices(services);

	auto expected_params   = GattReadCallbackParams {};
	expected_params.handle = characteristic.getValueHandle();

	EXPECT_CALL(mock_service_1, onDataRequested(compareParams(expected_params)))
		.Times(std::size(service_1_characteristic_table));
	EXPECT_CALL(mock_service_2, onDataRequested(compareParams(expected_params)))
		.Times(std::size(service_2_characteristic_table));

	gatt_event_handler.onDataRead(expected_params);
}

TEST_F(CoreGattServerEventHandlerTest, onDataRequestedParamsHandleDifferent)
{
	auto characteristic_value		  = uint8_t {};
	auto characteristic				  = GattCharacteristic {0x1234, &characteristic_value};
	auto service_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});
	auto mock_service				  = mock::BLEService(0x01, service_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service});

	gatt_event_handler.setServices(services);

	auto params	  = GattReadCallbackParams {};
	params.handle = characteristic.getValueHandle() + 1;

	EXPECT_CALL(mock_service, onDataRequested).Times(0);

	gatt_event_handler.onDataRead(params);
}
