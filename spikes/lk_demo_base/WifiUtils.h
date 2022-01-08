// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_WIFI_UTILS_H_
#define _LEKA_OS_WIFI_UTILS_H_

// #include "drivers/DigitalIn.h"
// #include "events/EventQueue.h"

#include "CoreWifi.h"
#include "ESP8266Interface.h"
#include "https_request.h"
#include "netsocket/TCPSocket.h"

namespace leka {

class WifiUtils
{
  public:
	explicit WifiUtils() = default;

	auto connect(const char *ssid, const char *pass) -> bool;
	void disconnect();
	void pingPage(const char *url);

  private:
	mbed::DigitalOut wifi_enable {WIFI_ENABLE, 1};
	CoreESP8266 wifi_module {WIFI_USART_TX, WIFI_USART_RX, false, WIFI_USART_RTS, WIFI_USART_CTS, WIFI_RESET};
	leka::CoreWifi corewifi {wifi_module, wifi_enable};

	std::string _url;
};

}	// namespace leka

#endif	 // _LEKA_OS_WIFI_UTILS_H_
