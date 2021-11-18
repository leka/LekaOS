// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BLEGap.h"

#include "LogKit.h"

using namespace leka;

void BLEGap::setDeviceName(const lstd::span<const char> name)
{
	_device_name = name;
}

void BLEGap::start()
{
	if (_ble.hasInitialized()) {
		return;
	}

	_ble_gap.setEventHandler(this);

	_ble.onEventsToProcess({this, &BLEGap::scheduleBLEEvents});

	if (auto error = _ble.init(this, &BLEGap::onInitComplete); error != BLE_ERROR_NONE) {
		return;
	}

	_event_queue.dispatch_forever();
}

void BLEGap::stop()
{
	if (_ble.hasInitialized()) {
		_ble.shutdown();
	}
}

void BLEGap::onInit(mbed::Callback<void(BLE &, events::EventQueue &)> cb)
{
	_post_init_cb = cb;
}

void BLEGap::onConnect(mbed::Callback<void(BLE &, events::EventQueue &, const ble::ConnectionCompleteEvent &event)> cb)
{
	_post_connect_cb = cb;
}

void BLEGap::onInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
	if (params->error != BLE_ERROR_NONE) {
		log_error("Ble initialization failed.");
		return;
	}

	startActivity();

	if (_post_init_cb) {
		_post_init_cb(_ble, _event_queue);
	}
}

void BLEGap::onConnectionComplete(const ble::ConnectionCompleteEvent &event)
{
	if (event.getStatus() == BLE_ERROR_NONE) {
		if (_post_connect_cb) {
			_post_connect_cb(_ble, _event_queue, event);
		}
	} else {
		startActivity();
	}
}

void BLEGap::onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event)
{
	startActivity();
}

void BLEGap::onAdvertisingEnd(const ble::AdvertisingEndEvent &event)
{
	startActivity();
}

void BLEGap::scheduleBLEEvents(BLE::OnEventsToProcessCallbackContext *event)
{
	_event_queue.call(mbed::callback(&event->ble, &BLE::processEvents));
}

void BLEGap::startAdvertising()
{
	if (_ble_gap.isAdvertisingActive(_advertising_handle)) {
		// We are already advertising
		return;
	}

	ble::AdvertisingParameters advertising_params(ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
												  ble::adv_interval_t(ble::millisecond_t(40)));
	advertising_params.setTxPower(-126);

	if (auto error = _ble_gap.setAdvertisingParameters(_advertising_handle, advertising_params);
		error != BLE_ERROR_NONE) {
		log_error("_ble.gap().setAdvertisingParameters() failed");
		return;
	}

	_advertising_data_builder.clear();
	// _advertising_data_builder.setAppearance(ble::adv_data_appearance_t::GENERIC_HEART_RATE_SENSOR);
	_advertising_data_builder.setFlags();
	// _advertising_data_builder.setTxPowerAdvertised(126);
	_advertising_data_builder.setName(_device_name.data());
	// const uint8_t specific_data[] = {0x2A, 0x2B, 0x2C, 0x2D};
	// _advertising_data_builder.setManufacturerSpecificData(specific_data);
	// _advertising_data_builder.setAdvertisingInterval(ble::adv_interval_t::min());
	// _advertising_data_builder.setConnectionIntervalPreference(ble::conn_interval_t::min(),
	// ble::conn_interval_t::max());
	// setServiceData // Some data in payload (not a real service)
	// setLocalServiceList // Available services

	if (auto error =
			_ble_gap.setAdvertisingPayload(_advertising_handle, _advertising_data_builder.getAdvertisingData());
		error != BLE_ERROR_NONE) {
		log_error("Gap::setAdvertisingPayload() failed");
		return;
	}

	if (auto error = _ble_gap.startAdvertising(_advertising_handle, ble::adv_duration_t(ble::millisecond_t(4000)));
		error != BLE_ERROR_NONE) {
		log_error("Gap::startAdvertising() failed");
		return;
	}
}
