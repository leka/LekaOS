#include "WebKit.h"
#include <algorithm>

#include "FileSystemKit.h"

using namespace leka;
using namespace std::chrono;

auto WebKit::connect(const char *ssid, const char *pass) -> bool
{
	auto wifi_network = leka::CoreWifi::Network {ssid, pass};

	auto is_connected = corewifi.connect(wifi_network);
	return is_connected;
}

void WebKit::setCertificateStore(std::span<const char *> certificates_path_list)
{
	std::copy(certificates_path_list.begin(), certificates_path_list.end(), _certificates_path_list.begin());
}

void WebKit::updateCertificate(const char *certificate_path)
{
	std::fill(_certificate.begin(), _certificate.end(), '\0');

	FileSystemKit::File _file_certificate {certificate_path};
	auto file_size = _file_certificate.size();

	_file_certificate.read(_certificate.begin(), file_size);

	_file_certificate.close();
}

auto WebKit::downloadFile(DownloadableFile const &downloadable_file) -> bool
{
	_url = downloadable_file.url;

	FileSystemKit::File _file;
	HttpResponse *response {nullptr};

	auto is_redirected				  = false;
	auto certificates_path_list_index = 0;

	auto did_download_file = [&is_redirected, &response] { return !is_redirected && response != nullptr; };
	auto did_use_all_certs = [this, &certificates_path_list_index] {
		return certificates_path_list_index >= std::size(_certificates_path_list);
	};
	auto should_download = [&did_download_file, &did_use_all_certs] {
		return !did_download_file() && !did_use_all_certs();
	};

	while (should_download()) {
		updateCertificate(_certificates_path_list.at(certificates_path_list_index));

		if (auto is_open = _file.open(downloadable_file.to_path, "w"); is_open) {
			auto save_to_file = [&_file](const char *string, uint32_t length) { _file.write(string, length); };

			HttpsRequest request(&corewifi, _certificate.data(), HTTP_GET, _url.data(), save_to_file);
			response = request.send();

			if (is_redirected = responseHasRedirectionURL(response); is_redirected) {
				getRedirectionURL(response);
				certificates_path_list_index = 0;
			} else {
				certificates_path_list_index++;
			}

			_file.close();
		}
	}

	return did_download_file();
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
