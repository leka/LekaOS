// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreBufferedSerial.h"
#include "LKCoreRFID.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

void readTag(RFIDTag tag)
{
	log_info("UID :");
	for (int i = 0; i < 8; ++i) {
		log_info(" %i,", tag.UID[i]);
	}

	log_info("CRC :");
	for (int i = 0; i < 2; ++i) {
		log_info(" %i,", tag.crc_UID[i]);
	}

	log_info("SAK :");
	for (int i = 0; i < 4; ++i) {
		log_info(" %i,", tag.UID[i]);
	}

	log_info("Data:");
	for (int i = 0; i < 16; ++i) {
		log_info(" %i,", tag.data[i]);
	}
}

void setProtocol(mbed::BufferedSerial &serial, LKCoreRFID &coreRfid)
{
	coreRfid.setProtocol();
	for (int i = 0; i < 10; i++) {
		if (serial.readable()) {
			coreRfid.receiveSetupAnswer();
			i = 10;
		}
	}
}

void setGain(mbed::BufferedSerial &serial, LKCoreRFID &coreRfid)
{
	coreRfid.setGain();
	for (int i = 0; i < 10; i++) {
		if (serial.readable()) {
			coreRfid.receiveSetupAnswer();
			i = 10;
		}
	}
}

bool sendREQA(mbed::BufferedSerial &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendREQA();
	rtos::ThisThread::sleep_for(10ms);
	for (int i = 0; i < 10; i++) {
		if (serial.readable()) {
			if (!coreRfid.receiveATQA()) {
				log_info("Reception failed\n");
				return false;
			};
			i = 10;
		}
	}
	return true;
}

void sendCL1(mbed::BufferedSerial &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendCL1();
	rtos::ThisThread::sleep_for(10ms);
	for (int i = 0; i < 10; i++) {
		if (serial.readable()) {
			coreRfid.receiveUID1();
			i = 10;
		}
	}
}

void sendUID1(mbed::BufferedSerial &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendUID1();
	rtos::ThisThread::sleep_for(10ms);
	for (int i = 0; i < 10; i++) {
		if (serial.readable()) {
			coreRfid.receiveSAK1();
			i = 10;
		}
	}
}

void sendCL2(mbed::BufferedSerial &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendCL2();
	rtos::ThisThread::sleep_for(10ms);
	for (int i = 0; i < 10; i++) {
		if (serial.readable()) {
			coreRfid.receiveUID2();
			i = 10;
		}
	}
}

void sendUID2(mbed::BufferedSerial &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendUID2();
	rtos::ThisThread::sleep_for(10ms);
	for (int i = 0; i < 10; i++) {
		if (serial.readable()) {
			coreRfid.receiveSAK2();
			i = 10;
		}
	}
}

void authentification(mbed::BufferedSerial &serial, LKCoreRFID &coreRfid)
{
	coreRfid.authentification();
	rtos::ThisThread::sleep_for(10ms);
	for (int i = 0; i < 10; i++) {
		if (serial.readable()) {
			coreRfid.receiveAuthentification();
			i = 10;
		}
	}
}

int main(void)
{
	static auto logserial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { logserial.write(str, size); });

	mbed::BufferedSerial serial(RFID_UART_TX, RFID_UART_RX, 57600);
	CoreBufferedSerial rfid_serial(serial);
	LKCoreRFID coreRfid(rfid_serial);

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	RFIDTag tag;

	setProtocol(serial, coreRfid);
	setGain(serial, coreRfid);

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;

		if (sendREQA(serial, coreRfid)) {
			sendCL1(serial, coreRfid);
			sendUID1(serial, coreRfid);
			sendCL2(serial, coreRfid);
			sendUID2(serial, coreRfid);
			authentification(serial, coreRfid);

			coreRfid.readRFIDTag();
			rtos::ThisThread::sleep_for(10ms);
			coreRfid.receiveRFIDTag();

			tag = coreRfid.getRFIDTag();
			readTag(tag);
		}
		rtos::ThisThread::sleep_for(1s);
	}
}
