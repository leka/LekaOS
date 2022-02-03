#include "BLEKit.h"

using namespace leka;

void BLEKit::init()
{
	if (_ble.hasInitialized()) {
		return;
	}

	_ble.onEventsToProcess({this, &BLEKit::processEvents});

	_event_queue.dispatch_forever();

	_ble.init();
}

void BLEKit::processEvents(BLE::OnEventsToProcessCallbackContext *context)
{
	_event_queue.call(mbed::callback(&context->ble, &BLE::processEvents));
}
