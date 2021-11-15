// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEGap.h"

using namespace leka;

void BLEGap::start()
{
	if (_ble.hasInitialized()) {
		return;
	}

	if (auto error = _ble.init(this, &BLEGap::onInitComplete); error != BLE_ERROR_NONE) {
		return;
	}
}

void BLEGap::stop()
{
	if (_ble.hasInitialized()) {
		_ble.shutdown();
	}
}

void BLEGap::onInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
	if (params->error != BLE_ERROR_NONE) {
		return;
	}

	startAdvertising();
}

void BLEGap::onConnectionComplete(const ble::ConnectionCompleteEvent &event)
{
	if (event.getStatus() != BLE_ERROR_NONE) {
		startAdvertising();
	}
}

void BLEGap::onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event)
{
	startAdvertising();
}

void BLEGap::onAdvertisingEnd(const ble::AdvertisingEndEvent &event)
{
	startAdvertising();
}

void BLEGap::startAdvertising()
{
	if (_ble_gap.isAdvertisingActive(ble::LEGACY_ADVERTISING_HANDLE)) {
		// We are already advertising
		return;
	}

	ble::AdvertisingParameters advertising_params(ble::advertising_type_t::CONNECTABLE_UNDIRECTED);
	if (auto error = _ble_gap.setAdvertisingParameters(ble::LEGACY_ADVERTISING_HANDLE, advertising_params);
		error != BLE_ERROR_NONE) {
		return;
	}

	constexpr auto MAX_ADVERTISING_PAYLOAD_SIZE {50};
	uint8_t _advertising_buffer[MAX_ADVERTISING_PAYLOAD_SIZE] {};
	ble::AdvertisingDataBuilder _advertising_data_builder {_advertising_buffer};
	_advertising_data_builder.clear();
	_advertising_data_builder.setFlags();
	_advertising_data_builder.setName("Leka_BLEGap");

	if (auto error = _ble_gap.setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE,
													_advertising_data_builder.getAdvertisingData());
		error != BLE_ERROR_NONE) {
		return;
	}

	if (auto error =
			_ble_gap.startAdvertising(ble::LEGACY_ADVERTISING_HANDLE, ble::adv_duration_t(ble::millisecond_t(4000)));
		error != BLE_ERROR_NONE) {
		return;
	}
}
