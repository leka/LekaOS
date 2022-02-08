#include "BLEKit.h"

using namespace leka;

void BLEKit::init()
{
	if (_ble.hasInitialized()) {
		return;
	}

	_core_gap.setEventHandler();

	_ble.onEventsToProcess({this, &BLEKit::processEvents});

	_core_gap.setDefaultAdvertising();
	_core_gap.setDeviceName("Leka");

	_core_gatt_server.setServer();
	// _core_gap.onInit(std::bind(&CoreGattServer::updateData, &_core_gatt_server));
	// _core_gap.onInit([this] { _core_gatt_server.updateData(); });	// clang suggestion

	_ble.init(&_core_gap, &CoreGap::onInitializationComplete);

	_event_queue.dispatch_forever();
}

void BLEKit::processEvents(BLE::OnEventsToProcessCallbackContext *context)
{
	_event_queue.callMbedCallback(mbed::callback(&context->ble, &BLE::processEvents));
}
