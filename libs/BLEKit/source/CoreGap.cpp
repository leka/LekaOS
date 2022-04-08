#include "CoreGap.h"

#include "internal/ServicesCharacteristics.h"

using namespace leka;
using namespace ble;

void CoreGap::setDefaultAdvertising()
{
	_advertising_data_builder.setServiceData(leka::service::commands::uuid, {{0}});
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
