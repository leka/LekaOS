#include "BLEKit.h"

using namespace leka;

void BLEKit::init()
{
	if (_ble.hasInitialized()) {
		return;
	}

	_ble.init();
}
