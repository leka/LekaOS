// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../BLE.h"

namespace ble {

auto BLE::initImplementation(FunctionPointerWithContext<ble::BLE::InitializationCompleteCallbackContext *> callback)
	-> ble_error_t
{
	leka::spy_ble_on_initialization_complete_callback = callback;
	leka::spy_ble_did_call_initialization			  = true;

	return BLE_ERROR_NONE;
}

auto BLE::hasInitialized() const -> bool
{
	return leka::spy_ble_hasInitialized_return_value;
}

void BLE::onEventsToProcess(FunctionPointerWithContext<ble::BLE::OnEventsToProcessCallbackContext *> const &callback)
{
	leka::spy_ble_on_events_to_process_callback = callback;
}

}	// namespace ble

namespace leka {

FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> spy_ble_on_initialization_complete_callback {};
bool spy_ble_did_call_initialization = false;

bool spy_ble_hasInitialized_return_value = false;

FunctionPointerWithContext<ble::BLE::OnEventsToProcessCallbackContext *> spy_ble_on_events_to_process_callback {};

void spy_ble_reset()
{
	spy_ble_did_call_initialization				= false;
	spy_ble_on_initialization_complete_callback = {};
	spy_ble_hasInitialized_return_value			= false;
	spy_ble_on_events_to_process_callback		= {};
}

}	// namespace leka
