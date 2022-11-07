#include "WebKit.h"
#include <algorithm>

#include "FileManagerKit.h"

using namespace leka;

auto WebKit::connect(const char *ssid, const char *pass) -> bool
{
	return _network.connect(ssid, pass);
}

void WebKit::setCertificateStore(std::span<const char *> certificates_path_list)
{
	std::copy(certificates_path_list.begin(), certificates_path_list.end(), _certificates_path_list.begin());
}

auto WebKit::updateCertificate(const char *certificate_path) -> bool
{
	std::fill(_certificate.begin(), _certificate.end(), '\0');

	if (auto is_open = _file_handle.open(certificate_path, "r"); is_open) {
		auto size = _file_handle.size();

		_file_handle.read(_certificate.begin(), size);

		_file_handle.close();
	}

	return _certificate[0] != '\0';
}

auto WebKit::downloadFile(DownloadableFile const &file) -> bool
{
	_url = file.url;

	auto response = HttpResponse {};

	auto is_redirected				  = false;
	auto certificates_path_list_index = std::size_t {0};

	auto did_download_file = [&is_redirected, &response] {
		return !is_redirected && std::size(response.headers_fields) != 0;
	};
	auto did_use_all_certs = [this, &certificates_path_list_index] {
		return certificates_path_list_index >= std::size(_certificates_path_list);
	};
	auto should_download = [&did_download_file, &did_use_all_certs] {
		return !did_download_file() && !did_use_all_certs();
	};

	while (should_download()) {
		if (auto valid_certificate = updateCertificate(_certificates_path_list.at(certificates_path_list_index));
			!valid_certificate) {
			certificates_path_list_index++;
			continue;
		}

		if (auto is_open = _file_handle.open(file.to_path, "w"); is_open) {
			auto save_to_file = [this](const char *string, uint32_t length) { _file_handle.write(string, length); };

			response = _network.sendRequest(_certificate.data(), _url.data(), save_to_file);

			if (is_redirected = responseHasRedirectionUrl(response); is_redirected) {
				getRedirectionUrl(response);
				certificates_path_list_index = 0;
			} else {
				certificates_path_list_index++;
			}

			_file_handle.close();
		} else {
			break;
		}
	}

	return did_download_file();
}

auto WebKit::responseHasRedirectionUrl(HttpResponse const &response) const -> bool
{
	if (std::size(response.headers_fields) == 0) {
		return false;
	}

	auto header_fields			 = response.headers_fields;
	auto contains_location_field = [](const std::string *field) { return *field == "Location"; };

	auto is_redirected = std::any_of(header_fields.begin(), header_fields.end(), contains_location_field);
	return is_redirected;
}

void WebKit::getRedirectionUrl(HttpResponse const &response)
{
	auto header_fields			 = response.headers_fields;
	auto contains_location_field = [](const std::string *field) { return *field == "Location"; };

	auto location_field_iterator = std::find_if(header_fields.begin(), header_fields.end(), contains_location_field);
	auto location_field_index	 = std::distance(header_fields.begin(), location_field_iterator);

	_url = *response.headers_values[location_field_index];
}

auto WebKit::getUrl() const -> const std::string &
{
	return _url;
}
