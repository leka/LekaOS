#include "WifiKit.h"

#include "Certificates.h"
#include "FileSystemKit.h"
#include "LogKit.h"
#include "https_request.h"

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
	// log_info("Downloading %s to %s ...", url, path);

	FileSystemKit::File _file;

	auto redirected = true;
	_url			= url;
	while (redirected) {
		if (auto is_open = _file.open(path, "wb"); is_open) {
			auto callback_write_data_in_file = [&_file](const char *string, uint32_t length) {
				auto wsize = _file.write((uint8_t *)string, length);
				// printf("Received chunk of size %ld, wrote %d :\n\r", length, wsize);
				// printf("%s\n\r", string);
				memset((char *)string, 0, length);
			};

			HttpsRequest request(&corewifi, CERTIFICATES.data(), HTTP_GET, _url.data(), callback_write_data_in_file);
			HttpResponse *response = request.send();
			// log_info("HTTPS GET request error code: %d", request.get_error());

			redirected = false;
			for (size_t ix = 0; ix < response->get_headers_length(); ix++) {
				if (auto is_redirected = !(response->get_headers_fields()[ix]->compare("Location")); is_redirected) {
					_url	   = *response->get_headers_values()[ix];
					redirected = true;
				}
			}

			_file.close();
		}
	}
}
