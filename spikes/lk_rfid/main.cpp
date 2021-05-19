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

void readTag(RFIDTag const tag)
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

void setProtocol(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	coreRfid.setProtocol();
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	coreRfid.receiveSetupAnswer();
}

void setGain(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	coreRfid.setGain();
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	coreRfid.receiveSetupAnswer();
}

bool sendREQA(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendREQA();
	rtos::ThisThread::sleep_for(10ms);
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	if (!coreRfid.receiveATQA()) {
		log_info("Reception failed\n");
		return false;
	};
	return true;
}

void sendCL1(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendCL1();
	rtos::ThisThread::sleep_for(10ms);
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	coreRfid.receiveUID1();
}

void sendUID1(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendUID1();
	rtos::ThisThread::sleep_for(10ms);
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	coreRfid.receiveSAK1();
}

void sendCL2(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendCL2();
	rtos::ThisThread::sleep_for(10ms);
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	coreRfid.receiveUID2();
}

void sendUID2(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	coreRfid.sendUID2();
	rtos::ThisThread::sleep_for(10ms);
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	coreRfid.receiveSAK2();
}

void authentification(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	coreRfid.authentification();
	rtos::ThisThread::sleep_for(10ms);
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	coreRfid.receiveAuthentification();
}

void readRFIDTag(mbed::BufferedSerial const &serial, LKCoreRFID &coreRfid)
{
	RFIDTag tag;
	coreRfid.readRFIDTag();
	rtos::ThisThread::sleep_for(10ms);
	while (!serial.readable()) {
		rtos::ThisThread::sleep_for(1ms);
	}
	coreRfid.receiveRFIDTag();
	tag = coreRfid.getRFIDTag();
	readTag(tag);
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
			readRFIDTag(serial, coreRfid);
		}
		rtos::ThisThread::sleep_for(1s);
	}
}
