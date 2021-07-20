// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/Thread.h"

#include "CoreWifi.h"
#include "ESP8266Interface.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "netsocket/TCPSocket.h"

using namespace leka;
using namespace std::chrono;

constexpr auto network_ssid		= "USER_SSID";
constexpr auto network_password = "USER_PASSWORD";

void runExample(WiFiInterface &wifi_interface, mbed::BufferedSerial &serial);

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(CONSOLE_TX, CONSOLE_RX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	auto hello = HelloWorld();

	auto wifi_enable  = mbed::DigitalOut {WIFI_ENABLE, 1};
	auto wifi_module  = CoreESP8266 {WIFI_USART_TX, WIFI_USART_RX, false, WIFI_USART_RTS, WIFI_USART_CTS, WIFI_RESET};
	auto corewifi	  = leka::CoreWifi {wifi_module, wifi_enable};
	auto wifi_network = leka::CoreWifi::Network {network_ssid, network_password};

	rtos::ThisThread::sleep_for(2s);

	if (auto is_not_connected = !corewifi.connect(wifi_network); is_not_connected) {
		log_error("Wifi connection failed");
		return 0;
	}

	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(500ms);

		runExample(corewifi, serial);
		rtos::ThisThread::sleep_for(500ms);
	}
}

void runExample(WiFiInterface &wifi_interface, mbed::BufferedSerial &serial)
{
	// Get information online
	// Open a socket on the network interface, and create a TCP connection to ifconfig.io
	TCPSocket socket;
	socket.open(&wifi_interface);

	SocketAddress socket_address;
	wifi_interface.get_ip_address(&socket_address);
	wifi_interface.gethostbyname("ifconfig.io", &socket_address);
	socket_address.set_port(80);
	socket.connect(socket_address);

	// Send a simple http request
	const auto tx_buffer = std::string("GET / HTTP/1.1\r\nHost: ifconfig.io\r\n\r\n");
	socket.send(tx_buffer.data(), tx_buffer.size());

	// Receive a simple http response and print out the response line
	std::string rx_buffer(32, ' ');
	auto available_data = socket.recv(rx_buffer.data(), rx_buffer.size());
	do {
		serial.write(rx_buffer.data(), available_data);	  // Print only received data
		available_data = socket.recv(rx_buffer.data(), rx_buffer.size());
	} while (available_data > 0);
	serial.write("\n", 1);

	// Close the socket to return its memory and bring down the network interface
	socket.close();
}
