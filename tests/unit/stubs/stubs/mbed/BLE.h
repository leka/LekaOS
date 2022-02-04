// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ble/BLE.h"
#include "ble/Gap.h"

namespace leka {

extern FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *>
	spy_ble_on_initialization_complete_callback;
extern bool spy_ble_did_call_initialization;

extern bool spy_ble_hasInitialized_return_value;

extern FunctionPointerWithContext<ble::BLE::OnEventsToProcessCallbackContext *> spy_ble_on_events_to_process_callback;

void spy_ble_reset();

}	// namespace leka
