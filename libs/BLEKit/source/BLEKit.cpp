#include "BLEKit.h"

using namespace leka;

void BLEKit::init()
{
	if (_ble.hasInitialized()) {
		return;
	}

	_core_gap.setEventHandler();

	_ble.onEventsToProcess({this, &BLEKit::processEvents});

	_event_queue.dispatch_forever();

	_core_gap.setDefaultAdvertising();
	_core_gap.setDeviceName("Leka");

	_ble.init(&_core_gap, &CoreGap::onInitializationComplete);
}

void BLEKit::processEvents(BLE::OnEventsToProcessCallbackContext *context)
{
	_event_queue.call(mbed::callback(&context->ble, &BLE::processEvents));
}
