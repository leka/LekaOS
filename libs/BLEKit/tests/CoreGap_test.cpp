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
	auto data_builder = AdvertisingDataBuilder {{buffer.begin(), buffer.end()}};

	data_builder.setServiceData(service::commands::uuid, {{0}});

	EXPECT_CALL(mbed_mock_gap,
				setAdvertisingPayload(LEGACY_ADVERTISING_HANDLE, compareAdvertisingPayload(data_builder)))
		.Times(1);
	EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).WillOnce(Return(false));
	EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(1);
	EXPECT_CALL(mbed_mock_gap, setAdvertisingParameters).Times(1);

	coregap.setDefaultAdvertising();
	coregap.startAdvertising();
}

TEST_F(CoreGapTest, setDeviceName)
{
	auto expected_device_name = "LekaCoreGap";

	std::array<uint8_t, 64> buffer {};
	auto data_builder = AdvertisingDataBuilder {{buffer.begin(), buffer.end()}};

	data_builder.setName(expected_device_name);

	EXPECT_CALL(mbed_mock_gap,
				setAdvertisingPayload(LEGACY_ADVERTISING_HANDLE, compareAdvertisingPayload(data_builder)))
		.Times(1);
	EXPECT_CALL(mbed_mock_gap, isAdvertisingActive).WillOnce(Return(false));
	EXPECT_CALL(mbed_mock_gap, startAdvertising).Times(1);
	EXPECT_CALL(mbed_mock_gap, setAdvertisingParameters).Times(1);

	coregap.setDeviceName(expected_device_name);
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

TEST_F(CoreGapTest, onInitializationComplete)
{
	BLE::InitializationCompleteCallbackContext context = {ble, BLE_ERROR_NONE};

	expectStartAdvertisingCall(true);

	ble.init(&coregap, &CoreGap::onInitializationComplete);
	spy_ble_on_initialization_complete_callback(&context);

	// coregap.onInitializationComplete(&context);   // Alternative
}
