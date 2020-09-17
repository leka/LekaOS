/**
 * @file LekaWifi.h
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-17
 *
 * @copyright Copyright (c) 2020
 */
#ifndef _LK_OS_MBED_LEKAWIFI_H_
#define _LK_OS_MBED_LEKAWIFI_H_

#include "ESP8266Interface.h"
#include "LekaWifiPins.h"
#include "mbed.h"

class LekaWifi
{
  public:
	LekaWifi();
	~LekaWifi() {};

	void start(void);

    /* TO CHECK */
	const char *sec2str(nsapi_security_t sec);
	void scan_demo(WiFiInterface *wifi);
	void http_demo(NetworkInterface *net);
    /* END OF CHECK */

  private:
	ESP8266Interface _interface;
	DigitalOut _esp8266_reset;
	DigitalOut _esp8266_enable;

    char const *SSID = "HUAWEI P smart 2019";
    char const *PWD;
};

#endif
