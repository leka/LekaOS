// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_GAP_H_
#define _LEKA_OS_LIB_BLE_GAP_H_

#include "events/mbed_events.h"

#include "ble/BLE.h"
#include "ble/Gap.h"

namespace leka {

static constexpr uint16_t MAX_ADVERTISING_PAYLOAD_SIZE = 50;

class BLEGap : public ble::Gap::EventHandler
{
  public:
	explicit BLEGap(BLE &ble, events::EventQueue &event_queue)
		: _ble(ble), _ble_gap(ble.gap()), _advertising_data_builder(_advertising_buffer), _event_queue(event_queue) {};

	void start();
	void stop();

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

	uint8_t _advertising_buffer[MAX_ADVERTISING_PAYLOAD_SIZE] {};
	ble::AdvertisingDataBuilder _advertising_data_builder;
	ble::advertising_handle_t _advertising_handle = ble::LEGACY_ADVERTISING_HANDLE;

	events::EventQueue &_event_queue;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_GAP_H_
