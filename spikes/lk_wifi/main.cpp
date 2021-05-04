// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "CoreWifi.h"
#include "ESP8266Interface.h"
#include "FileManager.h"
#include "HelloWorld.h"
#include "http_request.h"

#define SD_MOUNT_PATH "sd"
// #define FULL_UPDATE_FILE_PATH "/" SD_MOUNT_PATH "/" MBED_CONF_APP_UPDATE_FILE
#define FULL_UPDATE_FILE_PATH "\"update.bin\""

using namespace leka;

HelloWorld hello;

ESP8266Interface wifi_interface(WIFI_USART_TX, WIFI_USART_RX);
CoreWifi leka_wifi(wifi_interface, WIFI_RESET, WIFI_ENABLE);

FileManager sd_card;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Thread wifi_thread;

// FILE *file;
FIL file;
size_t received			= 0;
size_t received_packets = 0;

void runCustomHttp()
{
	f_open(&file, "update.bin", FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
	// Get information online
	// Open a socket on the network interface, and create a TCP connection to ifconfig.io
	TCPSocket socket;
	socket.open(&wifi_interface);

	SocketAddress socket_address("192.168.1.37", 8000);
	socket.connect(socket_address);
	// Send a simple http request
	char tx_buffer[] = "GET /update.bin HTTP/1.1\r\nHost: 192.168.1.37\r\n\r\n";
	socket.send(tx_buffer, sizeof tx_buffer);

	// Recieve a simple http response and print out the response line
	uint8_t rx_buffer[1024];
	int received_data;
	int total_received_data {0};
	UINT nbBytesWritten {0};
	do {
		received_data = socket.recv(rx_buffer, sizeof rx_buffer);
		f_write(&file, rx_buffer, received_data, &nbBytesWritten);

		total_received_data += received_data;
		printf("Received data: %d bytes\n", total_received_data);
	} while (received_data > 0);

	// Close the socket to return its memory and bring down the network interface
	socket.close();

	f_close(&file);

	printf("Size is: %ld\n", f_size(&file));
	ThisThread::sleep_for(1min);
}

void store_fragment(const char *buffer, size_t size)
{
	UINT nbBytesWritten {0};
	f_write(&file, buffer, size, &nbBytesWritten);

	received += size;
	received_packets++;

	printf("Received %u bytes\n", received);
}

void check_for_update()
{
	f_open(&file, "update.bin", FA_READ | FA_WRITE | FA_CREATE_ALWAYS);

	HttpRequest *req =
		new HttpRequest(&wifi_interface, HTTP_GET, "http://192.168.xxx.xxx:8000/update.bin", &store_fragment);

	HttpResponse *res = req->send();
	if (!res) {
		printf("HttpRequest failed (error code %d)\n", req->get_error());
		return;
	}

	printf("Done downloading: %d - %s\n", res->get_status_code(), res->get_status_message().c_str());

	f_close(&file);

	delete req;

	printf("Size is: %ld\n", f_size(&file));

	printf("Rebooting...\n\n");

	NVIC_SystemReset();
}

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Investigation Day!\n\n");

	rtos::ThisThread::sleep_for(2s);

	leka_wifi.connect();
	hello.start();

	// sd_card.testWorkingToRemove();

	while (true) {
		auto t = Kernel::Clock::now() - start;

		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);

		runCustomHttp();
		// check_for_update();
	}
}
