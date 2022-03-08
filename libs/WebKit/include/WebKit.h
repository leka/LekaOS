// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "CoreWifi.h"
#include "ESP8266Interface.h"
#include "https_request.h"
#include "netsocket/TCPSocket.h"

namespace leka {

class WebKit
{
  public:
	struct DownloadableFile {
		const char *url;
		const char *to_path;
	};

	explicit WebKit() = default;

	auto connect(const char *ssid, const char *pass) -> bool;

	void setCertificateStore(std::span<const char *> certificates_path_list);

	auto downloadFile(DownloadableFile const &downloadable_file) -> bool;

  private:
	auto responseHasRedirectionURL(HttpResponse *response) const -> bool;
	void getRedirectionURL(HttpResponse *response);

	void updateCertificate(const char *certificate_path);

	mbed::DigitalOut wifi_enable {WIFI_ENABLE, 1};
	CoreESP8266 wifi_module {WIFI_USART_TX, WIFI_USART_RX, false, WIFI_USART_RTS, WIFI_USART_CTS, WIFI_RESET};
	leka::CoreWifi corewifi {wifi_module, wifi_enable};

	std::string _url;

	std::array<const char *, 5> _certificates_path_list {};
	std::array<char, 8192> _certificate {};
};

}	// namespace leka