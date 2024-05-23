// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace leka {

struct HttpResponse {
	std::vector<std::string *> headers_fields;
	std::vector<std::string *> headers_values;
};

namespace interface {

	class Network
	{
	  public:
		virtual ~Network() = default;

		virtual auto connect(const char *ssid, const char *pass) -> bool = 0;
		virtual auto sendRequest(const char *ssl_ca_pem, const char *url,
								 const std::function<void(const char *at, uint32_t length)> &on_response_callback)
			-> HttpResponse & = 0;
	};

}	// namespace interface

}	// namespace leka
