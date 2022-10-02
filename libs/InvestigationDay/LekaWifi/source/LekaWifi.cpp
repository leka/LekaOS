// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaWifi.h"

using namespace mbed;
using namespace std::chrono;

Wifi::Wifi() : _interface(WIFI_USART_TX, WIFI_USART_RX), _wifi_reset(WIFI_RESET, 0), _wifi_enable(WIFI_ENABLE, 1)
{
	rtos::ThisThread::sleep_for(1s);
	_wifi_reset = 1;
}

const char *Wifi::sec2str(nsapi_security_t security)
{
	switch (security) {
		case NSAPI_SECURITY_NONE:
			return "None";
		case NSAPI_SECURITY_WEP:
			return "WEP";
		case NSAPI_SECURITY_WPA:
			return "WPA";
		case NSAPI_SECURITY_WPA2:
			return "WPA2";
		case NSAPI_SECURITY_WPA_WPA2:
			return "WPA/WPA2";
		case NSAPI_SECURITY_UNKNOWN:
		default:
			return "Unknown";
	}
}

void Wifi::scan_available_networks(WiFiInterface *wifi)
{
	WiFiAccessPoint *ap;

	printf("Scan:\n");

	int count = wifi->scan(NULL, 0);

	/* Limit number of network arbitrary to 15 */
	count = count < 15 ? count : 15;

	ap = new WiFiAccessPoint[15];

	count = wifi->scan(ap, count);
	for (int i = 0; i < count; i++) {
		printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\n", ap[i].get_ssid(),
			   sec2str(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
			   ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
	}
	printf("%d networks available.\n", count);

	delete[] ap;
}

void Wifi::connect_network(const char *network_name, const char *network_password)
{
	/* Connection to a specific network */
	printf("\nConnecting...\n");
	int ret = _interface.connect(network_name, network_password, NSAPI_SECURITY_WPA_WPA2);
	if (ret != 0) {
		printf("\nConnection error\n");
		return;
	}

	/* Get information of device on specific network */
	SocketAddress socket_address;

	printf("Success\n\n");
	printf("MAC: %s\n", _interface.get_mac_address());
	_interface.get_ip_address(&socket_address);
	printf("IP: %s\n", socket_address.get_ip_address());
	_interface.get_netmask(&socket_address);
	printf("Netmask: %s\n", socket_address.get_ip_address());
	_interface.get_gateway(&socket_address);
	printf("Gateway: %s\n", socket_address.get_ip_address());
	printf("RSSI: %d\n\n", _interface.get_rssi());

	/* Get information online (_old http_demo) */
	// Open a socket on the network interface, and create a TCP connection to mbed.org
	TCPSocket socket;
	socket.open(&_interface);

	// SocketAddress a;
	_interface.gethostbyname("ifconfig.io", &socket_address);
	socket_address.set_port(80);
	socket.connect(socket_address);
	// Send a simple http request
	char tx_buffer[] = "GET / HTTP/1.1\r\nHost: ifconfig.io\r\n\r\n";
	int tx_count	 = socket.send(tx_buffer, sizeof tx_buffer);
	printf("sent %d [%.*s]\n", tx_count, strstr(tx_buffer, "\n") - tx_buffer, tx_buffer);

	// Recieve a simple http response and print out the response line
	char rx_buffer[64];
	int rx_count = socket.recv(rx_buffer, sizeof rx_buffer);
	printf("recv %d [%.*s]\n", rx_count, strstr(rx_buffer, "\n") - rx_buffer, rx_buffer);

	// Close the socket to return its memory and bring down the network interface
	socket.close();

	_interface.disconnect();
}

void Wifi::start()
{
	/* Test based on Wi-Fi example on os.mbed.com */
	printf("WiFi example\n\n");

	while (true) {
		scan_available_networks(&_interface);
		// connect_network(_network_name, _network_password);
		rtos::ThisThread::sleep_for(10s);
	}

	printf("End of WiFi example\n\n");
}
