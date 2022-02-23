// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>

#include "ble/BLE.h"
#include "ble/Gap.h"

#include "CoreGapEventHandler.h"

namespace leka {

class CoreGap
{
  public:
	explicit CoreGap(ble::Gap &gap) : _gap(gap) {};

	void setDefaultAdvertising();

	void setEventHandler();
	void onInitializationComplete(BLE::InitializationCompleteCallbackContext *params);

	void setDeviceName(const char *name);

	void startAdvertising();

  private:
	ble::advertising_handle_t _advertising_handle {ble::LEGACY_ADVERTISING_HANDLE};
	ble::AdvertisingParameters _advertising_params {};
	std::array<uint8_t, 64> _advertising_buffer {};
	ble::AdvertisingDataBuilder _advertising_data_builder {{_advertising_buffer.begin(), _advertising_buffer.end()}};

	CoreGapEventHandler _gap_event_handler;
	ble::Gap &_gap;
};

}	// namespace leka
