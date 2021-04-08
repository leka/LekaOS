// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "BLEUtils.h"

#include "MotorsUtils.h"

Thread thread_motors;

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Thread thread_ble_event_queue;

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context)
{
	event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
}

// This projet is intended to test different types of motors using BLE to control their movement
// An app such as NRF Connect has to be installed on your iPhone/iPad
// When scanning, you should be able to discover the robot named "LekaDev"
// When connected, you'll have access to the `0xA000` sercie, with two characteristics:
//
//     - `0xA001` --> the motor will roll, turn, spin, roll backward
//     - `0xA002` --> the robot will accelerate and decelerate progressively
//
// To start/stop each behavior, you need to write `bool = true` or `byte = 0x01` to the behavior
// you want to test.

int main(void)
{
	printf("\nHello, BLE Motors Test!\n\n");

	BLE &ble = BLE::Instance();

	ble.onEventsToProcess(schedule_ble_events);

	MotorsBLEProgram motors_ble(ble, event_queue);

	motors_ble.start();

	thread_ble_event_queue.start(callback(&event_queue, &EventQueue::dispatch_forever));

	printf("\nBLE Ready!\n\n");

	thread_motors.start(motor_thread);

	while (true) {
		rtos::ThisThread::sleep_for(1s);
	}
}
