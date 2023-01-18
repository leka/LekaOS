#include "CoreNetwork.h"

#include "platform/Callback.h"

using namespace leka;

auto CoreNetwork::connect(const char *ssid, const char *pass) -> bool
{
	auto network = CoreWifi::Network {ssid, pass};

	auto is_connected = _corewifi.connect(network);
	return is_connected;
}

auto CoreNetwork::sendRequest(const char *ssl_ca_pem, const char *url,
							  const std::function<void(const char *at, uint32_t length)> &on_response_callback)
	-> HttpResponse &
{
	auto mbed_callback = mbed::Callback<void(const char *, uint32_t)>(
		[&on_response_callback](const char *at, uint32_t length) { on_response_callback(at, length); });

	_request = make_unique<HttpsRequest>(&_corewifi, ssl_ca_pem, HTTP_GET, url, mbed_callback);

	if (auto *mbed_http_response = _request->send(); mbed_http_response == nullptr) {
		_response.headers_fields = std::vector<std::string *> {};
		_response.headers_values = std::vector<std::string *> {};
	} else {
		_response.headers_fields = mbed_http_response->get_headers_fields();
		_response.headers_values = mbed_http_response->get_headers_values();
	}

	return _response;
}
