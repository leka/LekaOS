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
	const auto header_fields  = response->get_headers_fields();
	auto containLocationField = [](const std::string *header_field) { return *header_field == "Location"; };

	auto is_redirected = std::any_of(header_fields.begin(), header_fields.end(), containLocationField);
	return is_redirected;
}

void WifiKit::updateURL(HttpResponse *response)
{
	const auto header_fields  = response->get_headers_fields();
	auto containLocationField = [](const std::string *header_field) { return *header_field == "Location"; };

	auto location_field_iterator = std::find_if(header_fields.begin(), header_fields.end(), containLocationField);
	auto location_field_index	 = std::distance(header_fields.begin(), location_field_iterator);

	_url = *response->get_headers_values()[location_field_index];
}
