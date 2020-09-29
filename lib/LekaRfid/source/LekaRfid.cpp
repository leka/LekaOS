/**
 * @file LekaRfid.cpp
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-29
 *
 * @copyright Copyright (c) 2020
 */

#include "LekaRfid.h"

RFID::RFID() {}

void RFID::start()
{
	printf("RFID example\n\n");

	while (true) {
		ThisThread::sleep_for(10s);
	}

	printf("End of RFID example\n\n");
}
