#include "WifiUtils.h"

#include "rtos/ThisThread.h"

#include "Certificates.h"
#include "FileSystemKit.h"
#include "Flags.h"

using namespace leka;
using namespace std::chrono_literals;

auto WifiUtils::connect(const char *ssid, const char *pass) -> bool
{
	auto wifi_network = leka::CoreWifi::Network {ssid, pass};

	auto is_connected = corewifi.connect(wifi_network);
	return is_connected;
}

void WifiUtils::disconnect()
{
	corewifi.disconnect();
}

void WifiUtils::downloadFile(const char *url)
{
	_url = url;

	HttpsRequest request(&corewifi, CERTIFICATES.data(), HTTP_GET, _url.data());
	request.send();
}
