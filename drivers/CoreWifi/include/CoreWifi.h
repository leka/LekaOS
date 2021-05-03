// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_WIFI_DRIVER_H_
#define _LEKA_OS_DRIVER_LK_CORE_WIFI_DRIVER_H_

#include "drivers/DigitalOut.h"

#include "netsocket/WiFiInterface.h"

namespace leka {

class CoreWifi
{
  public:
	CoreWifi(WiFiInterface &interface, PinName reset_pin, PinName enable_pin);

	bool connect();
	void disconnect();

  private:
	WiFiInterface &_interface;
	mbed::DigitalOut _nreset;
	mbed::DigitalOut _enable;

	std::string _network_name	  = "";
	std::string _network_password = "";
};
}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_WIFI_DRIVER_H_
