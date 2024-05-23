// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGap.h"

#include "ble_mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "internal/ServicesCharacteristics.h"
#include "stubs/mbed/BLE.h"

using namespace leka;
using namespace ble;

using ::testing::_;
using ::testing::Return;
using ::testing::Sequence;

class CoreGapTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		ble::init_mocks();

		EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(1);
		coregap.setEventHandler();
	}
	void TearDown() override { ble::delete_mocks(); }

	BLE &ble			   = BLE::Instance();
	GapMock &mbed_mock_gap = gap_mock();
	CoreGap coregap {ble.gap()};

	void expectStartAdvertisingCall(bool expected)
	{
		if (expected) {
			EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).WillOnce(Return(false));
			EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(1);
			EXPECT_CALL(mbed_mock_gap, setAdvertisingParameters).Times(1);
			EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(1);
		} else {
			EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).Times(0);
			EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(0);
			EXPECT_CALL(mbed_mock_gap, setAdvertisingParameters).Times(0);
			EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(0);
		}
	};
};

MATCHER_P(compareAdvertisingParameters, expected_params, "")
{
	bool same_advertising_type = arg.getType() == expected_params.getType();
	bool same_tx_power		   = arg.getTxPower() == expected_params.getTxPower();

	bool result_are_identical = same_advertising_type && same_tx_power;

	return result_are_identical;
}

MATCHER_P(compareAdvertisingPayload, expected_data_builder, "")
{
	return expected_data_builder.getAdvertisingData() == arg;
}

MATCHER_P(compareSlaveLatency, expected_slave_latency, "")
{
	return expected_slave_latency.value() == arg.value();
}

TEST_F(CoreGapTest, initialization)
{
	EXPECT_NE(&coregap, nullptr);
}

TEST_F(CoreGapTest, setEventHandler)
{
	EXPECT_CALL(mbed_mock_gap, setEventHandler).Times(1);

	coregap.setEventHandler();
}

TEST_F(CoreGapTest, defaultAdvertisingParameters)
{
	auto params = AdvertisingParameters {};

	EXPECT_CALL(mbed_mock_gap,
				setAdvertisingParameters(LEGACY_ADVERTISING_HANDLE, compareAdvertisingParameters(params)))
		.Times(1);
	EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).WillOnce(Return(false));
	EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(1);
	EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(1);

	coregap.startAdvertising();
}

TEST_F(CoreGapTest, defaultAdvertisingPayload)
{
	std::array<uint8_t, 64> buffer {};
	auto data_builder			  = AdvertisingDataBuilder {{buffer.begin(), buffer.end()}};
	auto default_advertising_data = AdvertisingData {};

	data_builder.setName(default_advertising_data.name);
	data_builder.setServiceData(service::commands::uuid,
								{{default_advertising_data.battery, default_advertising_data.is_charging,
								  default_advertising_data.version_major, default_advertising_data.version_minor,
								  static_cast<uint8_t>(default_advertising_data.version_revision >> 8),
								  static_cast<uint8_t>(default_advertising_data.version_revision)}});

	EXPECT_CALL(mbed_mock_gap,
				setAdvertisingPayload(LEGACY_ADVERTISING_HANDLE, compareAdvertisingPayload(data_builder)))
		.Times(1);
	EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).WillOnce(Return(false));
	EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(1);
	EXPECT_CALL(mbed_mock_gap, setAdvertisingParameters).Times(1);

	coregap.setDefaultAdvertising();
	coregap.startAdvertising();
}

TEST_F(CoreGapTest, startAdvertisingAdvertisingWasInactive)
{
	Sequence seq1, seq2;

	EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).InSequence(seq1, seq2).WillOnce(Return(false));
	EXPECT_CALL(mbed_mock_gap, setAdvertisingParameters).Times(1).InSequence(seq1);
	EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(1).InSequence(seq2);
	EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(1).InSequence(seq1, seq2);

	coregap.startAdvertising();
}

TEST_F(CoreGapTest, startAdvertisingAdvertisingWasActive)
{
	EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).WillOnce(Return(true));
	EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(0);
	EXPECT_CALL(mbed_mock_gap, setAdvertisingParameters).Times(0);
	EXPECT_CALL(mbed_mock_gap, setAdvertisingPayload).Times(0);

	coregap.startAdvertising();
}

TEST_F(CoreGapTest, updateConnectionParameters)
{
	auto handle					 = connection_handle_t {};
	auto min_connection_interval = conn_interval_t {12};   // Min: 15ms = 12*1,25
	auto max_connection_interval = min_connection_interval;
	auto slave_latency			 = slave_latency_t {0};
	auto supervision_timeout	 = supervision_timeout_t {500};

	EXPECT_CALL(mbed_mock_gap,
				updateConnectionParameters(handle, min_connection_interval, max_connection_interval,
										   compareSlaveLatency(slave_latency), supervision_timeout, _, _));

	coregap.updateConnectionParameters(handle);
}

TEST_F(CoreGapTest, onInitializationComplete)
{
	BLE::InitializationCompleteCallbackContext context = {ble, BLE_ERROR_NONE};

	expectStartAdvertisingCall(true);

	ble.init(&coregap, &CoreGap::onInitializationComplete);
	spy_ble_on_initialization_complete_callback(&context);

	// coregap.onInitializationComplete(&context);   // Alternative
}

TEST_F(CoreGapTest, setAdvertising)
{
	std::array<uint8_t, 64> buffer {};
	auto data_builder		  = AdvertisingDataBuilder {{buffer.begin(), buffer.end()}};
	auto new_advertising_data = AdvertisingData {.name = "NewLeka", .battery = 0x42, .is_charging = 0x01};

	data_builder.setName(new_advertising_data.name);
	data_builder.setServiceData(service::commands::uuid, {new_advertising_data.data(), new_advertising_data.size()});

	EXPECT_CALL(mbed_mock_gap,
				setAdvertisingPayload(LEGACY_ADVERTISING_HANDLE, compareAdvertisingPayload(data_builder)))
		.Times(1);

	coregap.setAdvertising(new_advertising_data);
}

TEST_F(CoreGapTest, onConnectionCallback)
{
	auto dummy_on_connection_callback = std::function<void()> {};

	coregap.onConnectionCallback(dummy_on_connection_callback);

	// nothing expected
}

TEST_F(CoreGapTest, onDisconnectionCallback)
{
	auto dummy_on_disconnection_callback = std::function<void()> {};

	coregap.onDisconnectionCallback(dummy_on_disconnection_callback);

	// nothing expected
}

TEST_F(CoreGapTest, isConnectedDefault)
{
	auto is_connected = coregap.isConnected();

	EXPECT_FALSE(is_connected);
}

TEST_F(CoreGapTest, isConnectedAfterConnection)
{
	auto dummy_on_connection_callback = std::function<void()> {};

	coregap.onConnectionCallback(dummy_on_connection_callback);

	// nothing expected
}
