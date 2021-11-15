// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_GAP_H_
#define _LEKA_OS_LIB_BLE_GAP_H_

#include "ble/BLE.h"
#include "ble/Gap.h"

namespace leka {

class BLEGap : public ble::Gap::EventHandler
{
  public:
	explicit BLEGap(BLE &ble) : _ble(ble), _ble_gap(ble.gap()) {};

	void start();
	void stop();

  private:
	void onInitComplete(BLE::InitializationCompleteCallbackContext *params);
	void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override;
	void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) override;
	void onAdvertisingEnd(const ble::AdvertisingEndEvent &event) override;

	void startAdvertising();

	BLE &_ble;
	ble::Gap &_ble_gap;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_GAP_H_
