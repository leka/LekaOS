// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGattServer.h"

#include "ble_mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
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

	BLE &ble				  = BLE::Instance();
	GattServerMock &mock_gatt = gatt_server_mock();
	CoreGattServer coregattserver {ble.gattServer()};

	const uint8_t number_of_services		= 1;
	const uint8_t number_of_characteristics = 1;
};

TEST_F(CoreGattServerTest, initialization)
{
	EXPECT_NE(&coregattserver, nullptr);
}

TEST_F(CoreGattServerTest, setEventHandler)
{
	EXPECT_CALL(mock_gatt, setEventHandler).Times(1);

	coregattserver.setEventHandler();
}

TEST_F(CoreGattServerTest, registerServerWrite)
{
	EXPECT_CALL(mock_gatt, write(_, _, _, _)).Times(number_of_characteristics);
	EXPECT_CALL(mock_gatt, addService).Times(AnyNumber());

	coregattserver.registerServerWrite();
	coregattserver.setServices();

	coregattserver.updateData();
}

TEST_F(CoreGattServerTest, setServices)
{
	EXPECT_CALL(mock_gatt, addService).Times(number_of_services);
	EXPECT_CALL(mock_gatt, write(_, _, _, _)).Times(AnyNumber());

	coregattserver.registerServerWrite();
	coregattserver.setServices();

	coregattserver.updateData();
}

TEST_F(CoreGattServerTest, setServer)
{
	EXPECT_CALL(mock_gatt, setEventHandler).Times(1);
	EXPECT_CALL(mock_gatt, addService).Times(number_of_services);

	coregattserver.setServer();

	EXPECT_CALL(mock_gatt, write(_, _, _, _)).Times(number_of_characteristics);

	coregattserver.updateData();
}
