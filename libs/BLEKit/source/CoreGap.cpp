#include "CoreGap.h"

#include "internal/ServicesCharacteristics.h"

using namespace leka;
using namespace ble;

void CoreGap::setDefaultAdvertising()
{
	auto default_advertising_data = AdvertisingData {};

	_advertising_data_builder.setName(default_advertising_data.name);
	_advertising_data_builder.setServiceData(
		leka::service::commands::uuid,	 // ? for compatibility with Leka iOS app BLE implementation
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

void CoreGap::onConnectionCallback(const std::function<void()> &callback)
{
	_gap_event_handler.onConnectionCallback(callback);
}

void CoreGap::onDisconnectionCallback(const std::function<void()> &callback)
{
	_gap_event_handler.onDisconnectionCallback(callback);
}
