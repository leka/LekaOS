#include "CoreGap.h"

using namespace leka;
using namespace ble;

void CoreGap::setDefaultAdvertising()
{
	_advertising_data_builder.setAppearance(adv_data_appearance_t::GENERIC_HEART_RATE_SENSOR);
	_advertising_data_builder.setFlags();
	_advertising_data_builder.setManufacturerSpecificData({{0x2A, 0x2B, 0x2C, 0x2D}});
	_advertising_data_builder.setAdvertisingInterval(adv_interval_t::min());
	_advertising_data_builder.setServiceData(GattService::UUID_BATTERY_SERVICE, {{0x42}});
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

void CoreGap::setDeviceName(const char *name)
{
	_advertising_data_builder.setName(name);
}

void CoreGap::startAdvertising()
{
	if (_gap.isAdvertisingActive(_advertising_handle)) {
		return;
	}

	_gap.setAdvertisingParameters(_advertising_handle, _advertising_params);
	_gap.setAdvertisingPayload(_advertising_handle, _advertising_data_builder.getAdvertisingData());

	_gap.startAdvertising(_advertising_handle, adv_duration_t(millisecond_t(4000)));
}
