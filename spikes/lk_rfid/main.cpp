// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreBufferedSerial.h"
#include "HelloWorld.h"
#include "LKCoreRFID.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

HelloWorld hello;

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

int main(void)
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	mbed::BufferedSerial rfid_serial(RFID_UART_TX, RFID_UART_RX, 57600);
	CoreBufferedSerial rfidrfid_serial(rfid_serial);
	LKCoreRFID coreRfid(rfidrfid_serial);

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	hello.start();
	RFIDTag tag;

	coreRfid.setProtocol();
	for (int i = 0; i < 10; i++) {
		if (rfid_serial.readable()) {
			coreRfid.receiveSetupAnswer();
			i = 10;
		}
	}

	coreRfid.setGain();
	for (int i = 0; i < 10; i++) {
		if (rfid_serial.readable()) {
			coreRfid.receiveSetupAnswer();
			i = 10;
		}
	}

	while (true) {
		auto t	  = rtos::Kernel::Clock::now() - start;
		bool next = true;

		coreRfid.sendREQA();
		rtos::ThisThread::sleep_for(10ms);
		for (int i = 0; i < 10; i++) {
			if (rfid_serial.readable()) {
				if (!coreRfid.receiveATQA()) {
					log_info("Wrong ATQA\n");
					next = false;
				};
				i = 10;
			}
		}

		if (next) {
			coreRfid.sendCL1();
			rtos::ThisThread::sleep_for(10ms);
			for (int i = 0; i < 10; i++) {
				if (rfid_serial.readable()) {
					coreRfid.receiveUID1();
					i = 10;
				}
			}

			coreRfid.sendUID1();
			rtos::ThisThread::sleep_for(10ms);
			for (int i = 0; i < 10; i++) {
				if (rfid_serial.readable()) {
					coreRfid.receiveSAK1();
					i = 10;
				}
			}

			coreRfid.sendCL2();
			rtos::ThisThread::sleep_for(10ms);
			for (int i = 0; i < 10; i++) {
				if (rfid_serial.readable()) {
					coreRfid.receiveUID2();
					i = 10;
				}
			}

			coreRfid.sendUID2();
			rtos::ThisThread::sleep_for(10ms);
			for (int i = 0; i < 10; i++) {
				if (rfid_serial.readable()) {
					coreRfid.receiveSAK2();
					i = 10;
				}
			}

			coreRfid.authentification();
			rtos::ThisThread::sleep_for(10ms);
			for (int i = 0; i < 10; i++) {
				if (rfid_serial.readable()) {
					coreRfid.receiveAuthentification();
					i = 10;
				}
			}

			coreRfid.readRFIDTag();
			rtos::ThisThread::sleep_for(10ms);
			coreRfid.receiveRFIDTag();

			tag = coreRfid.getRFIDTag();
			readTag(tag);
		}
		rtos::ThisThread::sleep_for(1s);
	}
}
