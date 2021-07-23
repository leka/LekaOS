// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

#include "CoreAudio.h"
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

rtos::Thread vibrationsThread;
events::EventQueue vibrationsEventQueue;

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

	coreDAC.setCptCallbackPtr(&cptBufferCallback);
	coreDAC.setHalfCptCallbackPtr(&halfBufferCallback);

	coreDACTimer.initialize(SAMPLING_RATE);
	coreDAC.initialize();

	float lengthInSecs = 4;
	uint16_t length	   = lengthInSecs * SAMPLING_RATE;
	uint32_t freq	   = 60;
	float amplitude	   = 1.f;
	bool smoothTransi  = false;

	// std::vector<uint16_t> sinPeriod(SAMPLING_RATE / freq);
	// // sinPeriod.reserve(SAMPLING_RATE / freq);
	// fillBufferWithSinWave(sinPeriod.data(), sinPeriod.size(), freq);
	uint32_t samplesPerPeriod = SAMPLING_RATE / freq;
	uint16_t *sinPeriod		  = new uint16_t[samplesPerPeriod];

	// init globals

	_samplesPerPeriod  = samplesPerPeriod;
	_vibrationFinished = false;
	_endOfVibCounter   = length;

	createSinWavePeriod(sinPeriod, samplesPerPeriod);
	// printf("out of function\n");
	// for (uint32_t i = 0; i < samplesPerPeriod; ++i) {
	// 	printf("%d\n", sinPeriod[i]);
	// }

	auto *vibrationBuffer		= new uint16_t[samplesPerPeriod * 2];
	uint16_t *vibrationBuffer_2 = vibrationBuffer + samplesPerPeriod;

	for (uint32_t i = 0; i < samplesPerPeriod * 2; ++i) {
		vibrationBuffer[i] = sinPeriod[i % samplesPerPeriod] >> 4;
	}

	coreDAC.start(vibrationBuffer, samplesPerPeriod * 2);
	coreDACTimer.start();

	printf("Vib started\n");
	while (!_vibrationFinished) {
		rtos::ThisThread::sleep_for(50ms);
	}
	printf("Vib ended\n");

	coreDAC.stop();
	coreDACTimer.stop();

	coreDACTimer.deInitialize();
	coreDAC.deInitialize();
}

// std::array<EnvelopType, 4> types = {Window, Sawtooth, Triangular, Buzz};
// int numTimes					 = 10;
// for (auto t: types) {
// 	playPattern(_outBuffer.data(), OUT_BUFF_SIZE / 2, t, numTimes, coreDACTimer, coreDAC);
// 	rtos::ThisThread::sleep_for(2s);
// }
// rtos::ThisThread::sleep_for(4s);
// for (auto t: types) {
// 	playPattern(_outBuffer.data(), OUT_BUFF_SIZE, t, numTimes, coreDACTimer, coreDAC);
// 	rtos::ThisThread::sleep_for(2s);
// }