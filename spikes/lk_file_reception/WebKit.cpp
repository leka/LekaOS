#include "WebKit.h"
#include <ranges>

#include "FileSystemKit.h"

using namespace leka;
using namespace std::chrono;

auto WebKit::connect(const char *ssid, const char *pass) -> bool
{
	auto wifi_network = leka::CoreWifi::Network {ssid, pass};

	auto is_connected = corewifi.connect(wifi_network);
	return is_connected;
}

void WebKit::setCertificateStore(std::span<const char *> const &certificates_path_list)
{
	std::ranges::fill(_certificate_store, '\0');

	auto offset = 0;

	for (auto &certificate_path: certificates_path_list) {
		FileSystemKit::File _file_certificate {certificate_path};
		auto file_size = _file_certificate.size();

		_file_certificate.read(_certificate_store.begin() + offset, file_size);
		offset += file_size;

		_file_certificate.close();
	}
}

void WebKit::downloadFile(const char *url, const char *path)
{
	FileSystemKit::File _file;

	auto is_redirected = true;
	_url			   = url;
	while (is_redirected) {
		if (auto is_open = _file.open(path, "w"); is_open) {
			auto save_to_file = [&_file](const char *string, uint32_t length) { _file.write(string, length); };

			HttpsRequest request(&corewifi, _certificate_store.data(), HTTP_GET, _url.data(), save_to_file);
			HttpResponse *response = request.send();

			if (is_redirected = responseHasRedirectionURL(response); is_redirected) {
				getRedirectionURL(response);
			}

			_file.close();
		}
	}
}

auto WebKit::responseHasRedirectionURL(HttpResponse *response) const -> bool
{
	if (response == nullptr) {
		return false;
	}

	const auto header_fields	 = response->get_headers_fields();
	auto contains_location_field = [](const std::string *field) { return *field == "Location"; };

	auto is_redirected = std::any_of(header_fields.begin(), header_fields.end(), contains_location_field);
	return is_redirected;
}

void WebKit::getRedirectionURL(HttpResponse *response)
{
	const auto header_fields  = response->get_headers_fields();
	auto containLocationField = [](const std::string *header_field) { return *header_field == "Location"; };

	auto location_field_iterator = std::find_if(header_fields.begin(), header_fields.end(), containLocationField);
	auto location_field_index	 = std::distance(header_fields.begin(), location_field_iterator);

	_url = *response->get_headers_values()[location_field_index];
}
