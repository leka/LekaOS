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

void run()
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
	char tx_buffer[] = "GET / HTTP/1.1\r\nHost: ifconfig.io\r\n\r\n";
	socket.send(tx_buffer, sizeof tx_buffer);

	// Recieve a simple http response and print out the response line
	char rx_buffer[1];
	socket.recv(rx_buffer, sizeof rx_buffer);

	// Close the socket to return its memory and bring down the network interface
	socket.close();
}

void store_fragment(const char *buffer, size_t size)
{
	UINT nbBytesWritten {0};
	f_write(&file, buffer, size, &nbBytesWritten);

	received += size;
	received_packets++;

	// if (received_packets % 20 == 0) {
	printf("Received %u bytes\n", received);
	// }
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

	// printf("Write in doc...\n");
	// if (f_open(&file, "update.bin", FA_READ | FA_WRITE | FA_OPEN_APPEND | FA_CREATE_ALWAYS) != FR_OK) {
	// 	printf("Failed to open.\n");
	// 	return -1;
	// }

	// UINT nbBytesWritten {0};
	// if (f_write(&file, "hello", 5, &nbBytesWritten) != FR_OK) {
	// 	printf("Failed to Write.\n");
	// 	return -1;
	// }

	// if (f_close(&file) != FR_OK) {
	// 	printf("Failed to close.\n");
	// 	return -1;
	// }

	while (true) {
		auto t = Kernel::Clock::now() - start;

		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);

		// run();
		check_for_update();
	}
}
