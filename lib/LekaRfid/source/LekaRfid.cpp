// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaRFID.h"

RFID::RFID() : _interface(RFID_UART_TX, RFID_UART_RX, 57600) {}

bool RFID::echo()
{
	uint8_t buffer[1]						  = {0x00};
	const uint8_t aimed_buffer_length		  = 0x1;
	uint8_t aimed_buffer[aimed_buffer_length] = {0x55};

	_interface.write(_echo_cmd, _echo_cmd_length);
	ThisThread::sleep_for(10ms);

	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, aimed_buffer_length);
			// for (int i = 0; i < aimed_buffer_length; i++) {
			// 	printf("%X ", buffer[i]);
			// }
			// printf("\n");
			if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
				return true;
			}
		}
	}
	return false;
}

bool RFID::getID()
{
	uint8_t *buffer							  = new uint8_t[1];
	uint8_t buffer_length					  = 0;
	const uint8_t aimed_buffer_length		  = 0x11;
	uint8_t aimed_buffer[aimed_buffer_length] = {0x00, 0x0F, 0x4E, 0x46, 0x43, 0x20, 0x46, 0x53, 0x32,
												 0x4A, 0x41, 0x53, 0x54, 0x34, 0x00, 0x2A, 0xCE};

	_interface.write(_idn_cmd, _idn_cmd_length);
	ThisThread::sleep_for(10ms);

	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, 2);
			if (buffer[0] == 0x00) {
				buffer_length = buffer[1];
				_interface.read(buffer + 2, buffer_length);
				// for (int i = 0; i < aimed_buffer_length; i++) {
				// 	printf("%X ", buffer[i]);
				// }
				// printf("\n");
				if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool RFID::checkConnected()
{
	return echo();
}

void RFID::fieldOff()
{
	_interface.write(_field_off_cmd, _field_off_cmd_length);
	ThisThread::sleep_for(1ms);
}

bool RFID::setIEC15693()
{
	uint8_t buffer[2]						  = {0x00};
	const uint8_t aimed_buffer_length		  = 0x02;
	uint8_t aimed_buffer[aimed_buffer_length] = {0x00, 0x00};

	_interface.write(_iec_15693_cmd, _iec_15693_cmd_length);
	ThisThread::sleep_for(10ms);

	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, aimed_buffer_length);
			// for (int i = 0; i < aimed_buffer_length; i++) {
			// 	printf("%X ", buffer[i]);
			// }
			// printf("\n");
			if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
				return true;
			}
		}
	}
	return false;
}

bool RFID::setReceiverGain()
{
	uint8_t buffer[2]						  = {0x00};
	const uint8_t aimed_buffer_length		  = 0x2;
	uint8_t aimed_buffer[aimed_buffer_length] = {0x00, 0x00};

	_interface.write(_set_receiver_gain_cmd, _set_receiver_gain_cmd_length);
	ThisThread::sleep_for(10ms);

	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, aimed_buffer_length);
			for (int i = 0; i < aimed_buffer_length; i++) {
				printf("%X ", buffer[i]);
			}
			printf("\n");
			if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
				return true;
			}
		}
	}
	return false;
}

void RFID::sendReceive()
{
	uint8_t *buffer		  = new uint8_t[1];
	uint8_t buffer_length = 0;

	_interface.write(_send_receive_cmd, _send_receive_cmd_length);
	ThisThread::sleep_for(10ms);

	for (int i = 0; i < 10; i++) {
		if (_interface.readable()) {
			_interface.read(buffer, 2);
			buffer_length = buffer[1];
			_interface.read(buffer + 2, buffer_length);

			printf("Answer received from reader: ");
			for (int i = 0; i < buffer_length + 2; i++) {
				printf("%X ", buffer[i]);
			}
			printf("\n");
			return;
		}
	}
	printf("No answer received from reader...\n");
	return;
}

void RFID::sendReceive2()
{
    uint8_t *buffer          = new uint8_t[1];
    uint8_t buffer_length = 0;

    _interface.write(_send_receive2_cmd, _send_receive2_cmd_length);
    ThisThread::sleep_for(10ms);

    for (int i = 0; i < 10; i++) {
        if (_interface.readable()) {
            _interface.read(buffer, 2);
            buffer_length = buffer[1];
            _interface.read(buffer + 2, buffer_length);

            printf("Answer received from reader: ");
            for (int i = 0; i < buffer_length + 2; i++) {
                printf("%X ", buffer[i]);
            }
            printf("\n");
            return;
        }
    }
    printf("No answer received from reader...\n");
    return;
}

void RFID::sendReceive3()
{
    uint8_t *buffer          = new uint8_t[1];
    uint8_t buffer_length = 0;

    _interface.write(_send_receive3_cmd, _send_receive3_cmd_length);
    ThisThread::sleep_for(10ms);

    for (int i = 0; i < 10; i++) {
        if (_interface.readable()) {
            _interface.read(buffer, 2);
            buffer_length = buffer[1];
            _interface.read(buffer + 2, buffer_length);

            printf("Answer received from reader: ");
            for (int i = 0; i < buffer_length + 2; i++) {
                printf("%X ", buffer[i]);
            }
            printf("\n");
            return;
        }
    }
    printf("No answer received from reader...\n");
    return;
}

bool RFID::setIEC14443()
{
    uint8_t buffer[2]                          = {0x00};
    const uint8_t aimed_buffer_length          = 0x02;
    uint8_t aimed_buffer[aimed_buffer_length] = {0x00, 0x00};

    _interface.write(_iec_14443_cmd, _iec_14443_cmd_length);
    ThisThread::sleep_for(10ms);

    for (int i = 0; i < 10; i++) {
        if (_interface.readable()) {
            _interface.read(buffer, aimed_buffer_length);
            // for (int i = 0; i < aimed_buffer_length; i++) {
            //     printf("%X ", buffer[i]);
            // }
            // printf("\n");
            if ((memcmp(aimed_buffer, buffer, aimed_buffer_length) == 0)) {
                return true;
            }
        }
    }
    return false;
}

void RFID::start()
{
	printf("RFID example\n\n");

	while (!checkConnected()) {
		printf("RFID reader is not connected...\n");
		ThisThread::sleep_for(1s);
	}
	printf("RFID reader detected!\n\n");

    // while (!setIEC15693()) {
    //     printf("Attempt to enable RFID reader...\n");
    //     ThisThread::sleep_for(1s);
    // }
    // printf("RFID reader enable with IEC 15693!\n");

    while (!setIEC14443()) {
        printf("Attempt to enable RFID reader...\n");
        ThisThread::sleep_for(1s);
    }
    printf("RFID reader enable with IEC 14443!\n");

    while (!setReceiverGain()) {
        printf("Attempt to set RFID reader gain...\n");
        ThisThread::sleep_for(1s);
    }
    printf("RFID reader gain set!\n");

    while (true) {
        // sendReceive();
        sendReceive2();
        sendReceive3();
        ThisThread::sleep_for(10s);
    }

	printf("End of RFID example\n\n");
}
