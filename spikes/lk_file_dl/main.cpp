// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/Thread.h"

#define HTTP_RECEIVE_BUFFER_SIZE 128
#include "CoreWifi.h"
#include "FATFileSystem.h"
#include "FileSystemKit.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "https_request.h"
#include "netsocket/TCPSocket.h"

using namespace leka;
using namespace std::chrono;

constexpr auto WIFI_SSID = "SSID";
constexpr auto WIFI_PASS = "PASS";

const char CERTIFICATE[] =
	"-----BEGIN CERTIFICATE-----\n"
	"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
	"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
	"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
	"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
	"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
	"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
	"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
	"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
	"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
	"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
	"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
	"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
	"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
	"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
	"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
	"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
	"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
	"rqXRfboQnoZsG4q5WTP468SQvvG5\n"
	"-----END CERTIFICATE-----\n";

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

void runExample(WiFiInterface &wifi_interface, mbed::BufferedSerial &serial);
void downloadFile(WiFiInterface &wifi, const char *url, const char *path);
void downloadFileByChunks(WiFiInterface &wifi, const char *url, const char *path);

void initializeSD()
{
	sd_blockdevice.init();
	sd_blockdevice.frequency(25'000'000);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(CONSOLE_TX, CONSOLE_RX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	rtos::ThisThread::sleep_for(1s);

	log_info("Hello, World!\n");

	auto start = rtos::Kernel::Clock::now();

	initializeSD();

	auto wifi_enable  = mbed::DigitalOut {WIFI_ENABLE, 1};
	auto wifi_module  = CoreESP8266 {WIFI_USART_TX, WIFI_USART_RX, false, WIFI_USART_RTS, WIFI_USART_CTS, WIFI_RESET};
	auto corewifi	  = leka::CoreWifi {wifi_module, wifi_enable};
	auto wifi_network = leka::CoreWifi::Network {WIFI_SSID, WIFI_PASS};

	log_info("Connecting to %s:%s ...", WIFI_SSID, WIFI_PASS);
	if (!corewifi.connect(wifi_network)) {
		log_error("Wifi connection failed");
		return 0;
	}

	const char *url	 = "https://httpbin.org/get";
	const char *path = "/fs/somefiledownload.txt";

	log_info("Downloading %s to %s ...", url, path);
	downloadFileByChunks(corewifi, url, path);

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;

		// runExample(corewifi, serial);
		rtos::ThisThread::sleep_for(5s);
	}
}

void downloadFile(WiFiInterface &wifi, const char *url, const char *path)
{
	// TODO : check if path already exists in SD card and issue a warning if so

	// create the file
	FileSystemKit::File file;
	file.open(path, "wb");

	// connect to the url
	HttpsRequest request(&wifi, CERTIFICATE, HTTP_GET, url);
	// send https GET request
	HttpResponse *response = request.send();

	printf("Received response of size %d : \n\r%s\n\r", response->get_body_length(),
		   response->get_body_as_string().c_str());

	// write response body to file
	auto wsize = file.write((uint8_t *)response->get_body(), response->get_body_length());

	printf("Wrote %d bytes to %s\n\r", wsize, path);

	file.close();
}

void downloadFileByChunks(WiFiInterface &wifi, const char *url, const char *path)
{
	// TODO : check if path already exists in SD card and issue a warning if so

	// create the file
	FileSystemKit::File file;
	file.open(path, "wb");

	auto on_body_cb = [&file](const char *string, uint32_t length) {
		auto wsize = file.write((uint8_t *)string, length);
		printf("Received chunk of size %d, wrote %d :\n\r%s\n\r", length, wsize, string);
		memset((char *)string, 0, length);
	};
	// connect to the url
	HttpsRequest request(&wifi, CERTIFICATE, HTTP_GET, url, on_body_cb);
	// send https GET request
	HttpResponse *response = request.send();

	file.close();
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
