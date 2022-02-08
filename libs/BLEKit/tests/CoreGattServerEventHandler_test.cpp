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
	void SetUp() override
	{
		core_gatt_server_event_handler.registerServerWrite(mock_server_write_func.AsStdFunction());
		core_gatt_server_event_handler.setServices(_services);
	}
	void TearDown() override { ble::delete_mocks(); }

	BLE &ble = BLE::Instance();
	CoreGattServerEventHandler core_gatt_server_event_handler {};

	MockFunction<updateServiceFunction> mock_server_write_func;

	const UUID _uuid {0x0001};
	static const uint8_t _services_count {1};
	static const uint8_t _characteristics_count {1};
	uint8_t _characteristic_value {42};
	ReadOnlyGattCharacteristic<uint8_t> _writable_characteristic {0x1234, &_characteristic_value};
	std::array<GattCharacteristic *, _characteristics_count> _characteristic_table {&_writable_characteristic};
	mock::BLEService mock_service = mock::BLEService(_uuid, _characteristic_table, _characteristics_count);

	std::array<interface::BLEService *, _services_count> _services {&mock_service};
};

MATCHER_P(compareUpdateServiceFunction, expected_function, "")
{
	bool same_target_type = expected_function.target_type().name() == arg.target_type().name();

	return same_target_type;
}

MATCHER_P(compareGattWriteCallbackParams, expected_params, "")
{
	bool same_handle = expected_params.handle == arg.handle;

	return same_handle;
}

TEST_F(CoreGattServerEventHandlerTest, initialization)
{
	EXPECT_NE(&core_gatt_server_event_handler, nullptr);
}

TEST_F(CoreGattServerEventHandlerTest, registerServerWrite)
{
	updateServiceFunction dummy_function = [](GattAttribute::Handle_t, const uint8_t *, uint16_t) { return; };
	std::function<void()> impostor_function;

	core_gatt_server_event_handler.registerServerWrite(dummy_function);

	EXPECT_CALL(mock_service, getCharacteristicCount).WillOnce(Return(_characteristics_count));

	EXPECT_CALL(mock_service, updateData(compareUpdateServiceFunction(dummy_function))).Times(_characteristics_count);
	EXPECT_CALL(mock_service, updateData(compareUpdateServiceFunction(impostor_function))).Times(0);

	core_gatt_server_event_handler.updateData();
}

TEST_F(CoreGattServerEventHandlerTest, setServices)
{
	const UUID uuid {0x0001};
	static const uint8_t serivce_1_characteristics_count {1};
	static const uint8_t serivce_2_characteristics_count {2};
	static const uint8_t services_count {2};
	uint8_t characteristic_value {};
	ReadOnlyGattCharacteristic<uint8_t> writable_characteristic {0x1234, &characteristic_value};
	std::array<GattCharacteristic *, serivce_1_characteristics_count> service_1_characteristic_table {
		&writable_characteristic};
	std::array<GattCharacteristic *, serivce_2_characteristics_count> service_2_characteristic_table {
		&writable_characteristic, &writable_characteristic};
	mock::BLEService mock_service_1 =
		mock::BLEService(uuid, service_1_characteristic_table, serivce_1_characteristics_count);
	mock::BLEService mock_service_2 =
		mock::BLEService(uuid, service_2_characteristic_table, serivce_2_characteristics_count);

	std::array<interface::BLEService *, services_count> _services {&mock_service_1, &mock_service_2};

	core_gatt_server_event_handler.setServices(_services);

	EXPECT_CALL(mock_service_1, getCharacteristicCount).WillOnce(Return(serivce_1_characteristics_count));
	EXPECT_CALL(mock_service_1, updateData).Times(serivce_1_characteristics_count);
	EXPECT_CALL(mock_service_2, getCharacteristicCount).WillOnce(Return(serivce_2_characteristics_count));
	EXPECT_CALL(mock_service_2, updateData).Times(serivce_2_characteristics_count);

	core_gatt_server_event_handler.updateData();
}

TEST_F(CoreGattServerEventHandlerTest, onDataWritten)
{
	GattWriteCallbackParams dummy_params {};
	GattWriteCallbackParams impostor_params;
	dummy_params.handle = _writable_characteristic.getValueHandle();

	{
		InSequence seq;

		EXPECT_CALL(mock_service, getCharacteristicCount).WillOnce(Return(_characteristics_count));
		EXPECT_CALL(mock_service, getCharacteristic).WillOnce(Return(&_writable_characteristic));
		EXPECT_CALL(mock_service, onDataWritten(compareGattWriteCallbackParams(dummy_params)))
			.Times(_characteristics_count);
		EXPECT_CALL(mock_service, onDataWritten(compareGattWriteCallbackParams(impostor_params))).Times(0);
	}

	core_gatt_server_event_handler.onDataWritten(dummy_params);
}

TEST_F(CoreGattServerEventHandlerTest, onDataWrittenParamsHandleDifferent)
{
	GattWriteCallbackParams dummy_params {};
	dummy_params.handle = _writable_characteristic.getValueHandle() + 1;

	{
		InSequence seq;

		EXPECT_CALL(mock_service, getCharacteristicCount).WillOnce(Return(_characteristics_count));
		EXPECT_CALL(mock_service, getCharacteristic).WillOnce(Return(&_writable_characteristic));
		EXPECT_CALL(mock_service, onDataWritten).Times(0);
	}

	core_gatt_server_event_handler.onDataWritten(dummy_params);
}

TEST_F(CoreGattServerEventHandlerTest, updateData)
{
	{
		InSequence seq;

		EXPECT_CALL(mock_service, getCharacteristicCount).WillOnce(Return(_characteristics_count));
		EXPECT_CALL(mock_service, updateData).Times(_characteristics_count);
	}

	core_gatt_server_event_handler.updateData();
}
