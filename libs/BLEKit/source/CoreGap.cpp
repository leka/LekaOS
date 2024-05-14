// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// #include <BLETypes.h>
#include "CoreGap.h"

#include "LogKit.h"
#include "internal/ServicesCharacteristics.h"

using namespace leka;
using namespace ble;

void CoreGap::setDefaultAdvertising()
{
	auto default_advertising_data = AdvertisingData {};

	_advertising_data_builder.setName(default_advertising_data.name);

	// TODO(@leka/dev-embedded): current commands::uuid is based on Bluno board, change it to our own
	_advertising_data_builder.setServiceData(
		leka::service::commands::uuid,
		{default_advertising_data.data(), static_cast<int>(default_advertising_data.size())});
}

void CoreGap::setEventHandler()
{
	_gap_event_handler.registerStartAdvertising([this] { startAdvertising(); });

	_gap.setEventHandler(&_gap_event_handler);
}

void CoreGap::onInitializationComplete(BLE::InitializationCompleteCallbackContext *params)
{
	_gap_event_handler.onInitializationComplete(params);
}

void CoreGap::startAdvertising()
{
	using namespace std::chrono;

	if (_gap.isAdvertisingActive(_advertising_handle)) {
		return;
	}

	_gap.setAdvertisingParameters(_advertising_handle, _advertising_params);
	_gap.setAdvertisingPayload(_advertising_handle, _advertising_data_builder.getAdvertisingData());

	constexpr auto kAdvDuration = 4000ms;
	_gap.startAdvertising(_advertising_handle, adv_duration_t(kAdvDuration.count()));
}

void CoreGap::setAdvertising(AdvertisingData advertising_data)
{
	_advertising_data_builder.setName(advertising_data.name);
	_advertising_data_builder.setServiceData(
		leka::service::commands::uuid,	 // ? for compatibility with Leka iOS app BLE implementation
		{advertising_data.data(), static_cast<int>(advertising_data.size())});

	_gap.setAdvertisingPayload(_advertising_handle, _advertising_data_builder.getAdvertisingData());
}

void CoreGap::updateConnectionParameters(ble::connection_handle_t handle)
{
	// ? For mbed definitions, see:
	// ? mbed-os/connectivity/FEATURE_BLE/include/ble/Gap.h
	// ? For Apple guidelines, see:
	// ? https://developer.apple.com/accessories/Accessory-Design-Guidelines.pdf#page=221

	auto min_connection_interval = conn_interval_t {12};   // Min: 15ms = 12*1,25
	auto max_connection_interval = min_connection_interval;
	auto slave_latency			 = slave_latency_t {0};
	auto supervision_timeout	 = supervision_timeout_t {500};

	_gap.updateConnectionParameters(handle, min_connection_interval, max_connection_interval, slave_latency,
									supervision_timeout);
}

void CoreGap::onConnectionCallback(const std::function<void()> &callback)
{
	_on_connection_callback = [&, callback](connection_handle_t handle) {
		updateConnectionParameters(handle);
		callback();
		auto is_dple_supported =
			_gap.isFeatureSupported(ble::controller_supported_features_t::LE_DATA_PACKET_LENGTH_EXTENSION);
		log_info("DPLE supported: %s", is_dple_supported ? "true" : "false");
	};
	_gap_event_handler.onConnectionCallback(_on_connection_callback);
}

void CoreGap::onDisconnectionCallback(const std::function<void()> &callback)
{
	_gap_event_handler.onDisconnectionCallback(callback);
}

auto CoreGap::isConnected() const -> bool
{
	return _gap_event_handler.isConnected();
}
