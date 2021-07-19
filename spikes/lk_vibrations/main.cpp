// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

#include "HelloWorld.h"
#include "LKCoreSTM32Hal.h"
#include "VibrationsUtils.h"

using namespace leka;
using namespace std::chrono;

// uint16_t _outBuffer[OUT_BUFF_SIZE] = {0};

mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
std::array<uint16_t, OUT_BUFF_SIZE> _outBuffer;
auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

LKCoreSTM32Hal hal;
CoreDACTimer coreDACTimer(hal);
CoreDAC coreDAC(hal);

/**
 * @brief This function handles DMA1 stream5 global interrupt.
 */
extern "C" {
void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coreDAC.getHandle().DMA_Handle1);
}
}

auto main() -> int
{
	printf("\n\nStarting vibration process, hold your kids\n\n\n");

	coreDACTimer.initialize(SAMPLING_RATE);
	coreDAC.initialize();

	// fillBufferWithEnvelop(_outBuffer.data(), OUT_BUFF_SIZE, Buzz, 0xFFFF);
	// fillEnvelop(_outBuffer.data(), OUT_BUFF_SIZE, 0xFFFF);

	// coreDAC.start(_outBuffer.data(), OUT_BUFF_SIZE);
	// coreDACTimer.start();

	// int count = 0;
	// printf("Start of loop\n");
	// while (count < 20) {
	// 	if (coreDAC.dmaFlag() == CoreDAC::Cpt) {
	// 		count++;
	// 		coreDACTimer.stop();
	// 		wait_us(500000);
	// 		coreDACTimer.start();
	// 		printf("count : %d\n", count);
	// 		coreDAC.dmaFlag() = CoreDAC::None;
	// 	}
	// }
	// printf("End of loop\n");
	// coreDACTimer.stop();
	// coreDAC.stop();

	std::array<EnvelopType, 4> types = {Window, Sawtooth, Triangular, Buzz};
	int numTimes					 = 10;
	for (auto t: types) {
		playPattern(_outBuffer.data(), OUT_BUFF_SIZE / 2, t, numTimes, coreDACTimer, coreDAC);
		rtos::ThisThread::sleep_for(2s);
	}
	rtos::ThisThread::sleep_for(4s);
	for (auto t: types) {
		playPattern(_outBuffer.data(), OUT_BUFF_SIZE, t, numTimes, coreDACTimer, coreDAC);
		rtos::ThisThread::sleep_for(2s);
	}

	coreDACTimer.deInitialize();
	coreDAC.deInitialize();
}
