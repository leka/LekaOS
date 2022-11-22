// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGattServer.h"
#include <array>
#include <tuple>

#include "ble_mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/BLEService.h"
#include "stubs/mbed/BLE.h"

using namespace leka;
using namespace ble;

using ::testing::_;
using ::testing::AnyNumber;

class CoreGattServerTest : public testing::Test
{
  protected:
	void SetUp() override { ble::init_mocks(); }
	void TearDown() override { ble::delete_mocks(); }

	BLE &ble					   = BLE::Instance();
	GattServerMock &mbed_mock_gatt = gatt_server_mock();
	CoreGattServer gatt_server {ble.gattServer()};
};

ACTION_P(GetUpdateDataFunction, pointer)
{
	*pointer = interface::BLEService::data_to_send_handle_t(arg0);
}

TEST_F(CoreGattServerTest, initialization)
{
	EXPECT_NE(&gatt_server, nullptr);
}

TEST_F(CoreGattServerTest, setEventHandler)
{
	EXPECT_CALL(mbed_mock_gatt, setEventHandler).Times(1);

	gatt_server.setEventHandler();
}

TEST_F(CoreGattServerTest, setServices)
{
	auto characteristic_value		  = uint8_t {};
	auto characteristic				  = GattCharacteristic {0x1234, &characteristic_value};
	auto service_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});
	auto mock_service				  = mock::BLEService(0x01, service_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service});

	EXPECT_CALL(mbed_mock_gatt, addService).Times(std::size(services));

	gatt_server.setServices(services);
}

TEST_F(CoreGattServerTest, onDataReadyToSend)
{
	auto characteristic_value		  = uint8_t {};
	auto characteristic				  = GattCharacteristic {0x1234, &characteristic_value};
	auto service_characteristic_table = std::to_array<GattCharacteristic *>({&characteristic});
	auto mock_service				  = mock::BLEService(0x01, service_characteristic_table);

	auto services = std::to_array<interface::BLEService *>({&mock_service});

	EXPECT_CALL(mbed_mock_gatt, addService).Times(AnyNumber());

	gatt_server.setServices(services);

	auto handle		  = GattAttribute::Handle_t {};
	auto data_to_send = std::to_array<const uint8_t>({0x2A, 0x2B, 0x2C, 0x2D});

	auto tuple		   = std::make_tuple(handle, data_to_send);
	const auto &[h, d] = tuple;	  // need for the EXPECT_CALL: addresses must be the same

	EXPECT_CALL(mbed_mock_gatt, write(h, d.data(), std::size(d), _)).Times(1);

	mock_service.sendData(tuple);
}
