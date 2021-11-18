// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_GAP_H_
#define _LEKA_OS_LIB_BLE_GAP_H_

#include "events/mbed_events.h"

#include "ble/BLE.h"
#include "ble/Gap.h"

#include "cxxsupport/lstd_span"

namespace leka {

static constexpr uint16_t MAX_ADVERTISING_PAYLOAD_SIZE = 50;

class BLEGap : public ble::Gap::EventHandler
{
  public:
	explicit BLEGap(BLE &ble, events::EventQueue &event_queue)
		: _ble(ble), _ble_gap(ble.gap()), _advertising_data_builder(_advertising_buffer), _event_queue(event_queue) {};

	void setDeviceName(lstd::span<const char> name);

	void start();
	void stop();

	void onInit(mbed::Callback<void(BLE &, events::EventQueue &)> cb);
	void onConnect(mbed::Callback<void(BLE &, events::EventQueue &, const ble::ConnectionCompleteEvent &event)> cb);

  protected:
	virtual void startActivity()
	{
		_event_queue.call([this]() { startAdvertising(); });
	}

  private:
	void onInitComplete(BLE::InitializationCompleteCallbackContext *params);
	void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override;
	void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) override;
	void onAdvertisingEnd(const ble::AdvertisingEndEvent &event) override;

	void scheduleBLEEvents(BLE::OnEventsToProcessCallbackContext *event);

	void startAdvertising();
	// void stopAdvertising();

	BLE &_ble;
	ble::Gap &_ble_gap;

	lstd::span<const char> _device_name;

	uint8_t _advertising_buffer[MAX_ADVERTISING_PAYLOAD_SIZE] {};
	ble::AdvertisingDataBuilder _advertising_data_builder;
	ble::advertising_handle_t _advertising_handle = ble::LEGACY_ADVERTISING_HANDLE;

	events::EventQueue &_event_queue;

	mbed::Callback<void(BLE &, events::EventQueue &)> _post_init_cb;
	mbed::Callback<void(BLE &, events::EventQueue &, const ble::ConnectionCompleteEvent &event)> _post_connect_cb;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_GAP_H_
