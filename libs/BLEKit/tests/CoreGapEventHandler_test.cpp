// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreGapEventHandler.h"

#include "ble_mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;
using namespace ble;

using ::testing::MockFunction;

class CoreGapEventHandlerTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		core_gap_event_handler.registerStartAdvertising(mock_start_advertising_func.AsStdFunction());
	}
	void TearDown() override { ble::delete_mocks(); }

	BLE &ble = BLE::Instance();
	CoreGapEventHandler core_gap_event_handler {};

	MockFunction<void()> mock_start_advertising_func;
};

TEST_F(CoreGapEventHandlerTest, initialization)
{
	EXPECT_NE(&core_gap_event_handler, nullptr);
}

TEST_F(CoreGapEventHandlerTest, onInitializationCompleteSucess)
{
	BLE::InitializationCompleteCallbackContext context = {ble, BLE_ERROR_NONE};

	EXPECT_CALL(mock_start_advertising_func, Call).Times(1);

	core_gap_event_handler.onInitializationComplete(&context);
}

TEST_F(CoreGapEventHandlerTest, onInitializationCompleteFail)
{
	BLE::InitializationCompleteCallbackContext context = {ble, BLE_ERROR_INVALID_PARAM};

	EXPECT_CALL(mock_start_advertising_func, Call).Times(0);

	core_gap_event_handler.onInitializationComplete(&context);
}

TEST_F(CoreGapEventHandlerTest, onConnectionCompleteSuccess)
{
	auto connection_complete_event =
		ConnectionCompleteEvent(BLE_ERROR_NONE, INVALID_ADVERTISING_HANDLE, connection_role_t::CENTRAL,
								peer_address_type_t::ANONYMOUS, ble::address_t(), ble::address_t(), ble::address_t(),
								ble::conn_interval_t::max(), 0, ble::supervision_timeout_t::max(), 0);

	EXPECT_CALL(mock_start_advertising_func, Call).Times(0);

	core_gap_event_handler.onConnectionComplete(connection_complete_event);
}

TEST_F(CoreGapEventHandlerTest, onConnectionCompleteFail)
{
	auto connection_complete_event =
		ConnectionCompleteEvent(BLE_ERROR_BUFFER_OVERFLOW, INVALID_ADVERTISING_HANDLE, connection_role_t::CENTRAL,
								peer_address_type_t::ANONYMOUS, ble::address_t(), ble::address_t(), ble::address_t(),
								ble::conn_interval_t::max(), 0, ble::supervision_timeout_t::max(), 0);

	EXPECT_CALL(mock_start_advertising_func, Call).Times(1);

	core_gap_event_handler.onConnectionComplete(connection_complete_event);
}

TEST_F(CoreGapEventHandlerTest, DisconnectionCompleteEvent)
{
	auto handler = uintptr_t {};
	auto reason	 = disconnection_reason_t::AUTHENTICATION_FAILURE;

	auto disconnection_complete_event = DisconnectionCompleteEvent(handler, reason);

	EXPECT_CALL(mock_start_advertising_func, Call).Times(1);

	core_gap_event_handler.onDisconnectionComplete(disconnection_complete_event);
}

TEST_F(CoreGapEventHandlerTest, onAdvertisingEnd)
{
	EXPECT_CALL(mock_start_advertising_func, Call).Times(1);

	core_gap_event_handler.onAdvertisingEnd(AdvertisingEndEvent());
}

TEST_F(CoreGapEventHandlerTest, onConnectionCallback)
{
	MockFunction<void()> mock_on_connection_callback;

	core_gap_event_handler.onConnectionCallback(mock_on_connection_callback.AsStdFunction());

	EXPECT_CALL(mock_on_connection_callback, Call).Times(1);

	//

	auto connection_complete_event =
		ConnectionCompleteEvent(BLE_ERROR_BUFFER_OVERFLOW, INVALID_ADVERTISING_HANDLE, connection_role_t::CENTRAL,
								peer_address_type_t::ANONYMOUS, ble::address_t(), ble::address_t(), ble::address_t(),
								ble::conn_interval_t::max(), 0, ble::supervision_timeout_t::max(), 0);

	EXPECT_CALL(mock_start_advertising_func, Call).Times(1);

	core_gap_event_handler.onConnectionComplete(connection_complete_event);
}

TEST_F(CoreGapEventHandlerTest, onDisconnectionCallback)
{
	MockFunction<void()> mock_on_disconnection_callback;

	core_gap_event_handler.onDisconnectionCallback(mock_on_disconnection_callback.AsStdFunction());

	EXPECT_CALL(mock_on_disconnection_callback, Call).Times(1);

	//

	auto handler = uintptr_t {};
	auto reason	 = disconnection_reason_t::AUTHENTICATION_FAILURE;

	auto disconnection_complete_event = DisconnectionCompleteEvent(handler, reason);

	EXPECT_CALL(mock_start_advertising_func, Call).Times(1);

	core_gap_event_handler.onDisconnectionComplete(disconnection_complete_event);
}
