#include "WifiKit.h"

#include "Certificates.h"
#include "FileSystemKit.h"

using namespace leka;
using namespace std::chrono;

auto WifiKit::connect(const char *ssid, const char *pass) -> bool
{
	auto wifi_network = leka::CoreWifi::Network {ssid, pass};

	auto is_connected = corewifi.connect(wifi_network);
	return is_connected;
}

void WifiKit::downloadFile(const char *url, const char *path)
{
	FileSystemKit::File _file;

	auto is_redirected = true;
	_url			   = url;
	while (is_redirected) {
		if (auto is_open = _file.open(path, "w"); is_open) {
			auto callback_write_data_in_file = [&_file](const char *string, uint32_t length) {
				_file.write(string, length);
			};

			HttpsRequest request(&corewifi, CERTIFICATES.data(), HTTP_GET, _url.data(), callback_write_data_in_file);
			HttpResponse *response = request.send();

			if (is_redirected = isRedirected(response); is_redirected) {
				updateURL(response);
			}

			_file.close();
		}
	}
}

auto WifiKit::isRedirected(HttpResponse *response) const -> bool
{
	for (const auto *header_field: response->get_headers_fields()) {
		if (auto is_redirected = !(header_field->compare("Location")); is_redirected) {
			return true;
		}
	}
	return false;
}

void WifiKit::updateURL(HttpResponse *response)
{
	for (size_t ix = 0; ix < response->get_headers_length(); ix++) {
		if (auto header_contain_new_url = !(response->get_headers_fields()[ix]->compare("Location"));
			header_contain_new_url) {
			_url = *response->get_headers_values()[ix];
			break;
		}
	}
}
