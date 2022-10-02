// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

#include "drivers/DigitalOut.h"
#include "rtos/Thread.h"

#include "connectivity/netsocket/include/netsocket/TCPSocket.h"

#include "ESP8266Interface.h"

class Wifi
{
  public:
	Wifi();
	~Wifi() {};

	void start(void);
	void connect_network(const char *network_name, const char *network_password);

	static const char *sec2str(nsapi_security_t security);
	void scan_available_networks(WiFiInterface *wifi);
	// void http_demo(NetworkInterface *net);

  private:
	ESP8266Interface _interface;
	mbed::DigitalOut _wifi_reset;
	mbed::DigitalOut _wifi_enable;

	char const *_network_name = "HUAWEI P smart 2019";
	char const *_network_password;
};
