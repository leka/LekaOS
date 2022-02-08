// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_CORE_GAP_EVENT_HANDLER_H_
#define _LEKA_OS_LIB_CORE_GAP_EVENT_HANDLER_H_

#include "ble/BLE.h"
#include "ble/Gap.h"

namespace leka {

class CoreGapEventHandler : public ble::Gap::EventHandler
{
  public:
	explicit CoreGapEventHandler() = default;

	void registerStartAdvertising(std::function<void()> const &function);

	void onInitializationComplete(BLE::InitializationCompleteCallbackContext *params);

	void onConnectionComplete(ble::ConnectionCompleteEvent const &event) override;
	void onDisconnectionComplete(ble::DisconnectionCompleteEvent const &event) override;
	void onAdvertisingEnd(ble::AdvertisingEndEvent const &event) override;

  private:
	std::function<void()> _start_advertising {};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_CORE_GAP_EVENT_HANDLER_H_
