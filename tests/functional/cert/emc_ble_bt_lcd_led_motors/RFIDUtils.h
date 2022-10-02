#include "LekaRFID.h"

RFID leka_rfid;

void initRfid()
{
	while (!leka_rfid.setIEC14443()) {
		rtos::ThisThread::sleep_for(100ms);
	}
	while (!leka_rfid.setReceiverGain()) {
		rtos::ThisThread::sleep_for(100ms);
	}
}

size_t getRfid(char *response)
{
	char buff[48] {};

	leka_rfid.sendReceive(2);
	size_t length = leka_rfid.getAnswer(buff);
	for (size_t i = 0; i < length; i++) {
		sprintf((char *)(response + i * 3), "%02X ", buff[i]);
	}
	return length * 2;
}