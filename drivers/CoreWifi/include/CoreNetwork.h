// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreWifi.h"
#include "ESP8266Interface.h"
#include "https_request.h"
#include "interface/drivers/Network.h"

namespace leka {

class CoreNetwork : public interface::Network
{
  public:
	explicit CoreNetwork() = default;

	auto connect(const char *ssid, const char *pass) -> bool final;
	auto sendRequest(const char *ssl_ca_pem, const char *url,
					 const std::function<void(const char *at, uint32_t length)> &on_response_callback = nullptr)
		-> HttpResponse & final;

  private:
	mbed::DigitalOut _wifi_enable {WIFI_ENABLE, 1};
	CoreESP8266 _wifi_module {WIFI_USART_TX, WIFI_USART_RX, false, WIFI_USART_RTS, WIFI_USART_CTS, WIFI_RESET};
	CoreWifi _corewifi {_wifi_module, _wifi_enable};

	unique_ptr<HttpsRequest> _request;
	HttpResponse _response;
};

}	// namespace leka
