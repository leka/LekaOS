// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"
#include <sys/types.h>

#include "CoreDAC.h"
#include "CoreDACTimer.h"
#include "LKCoreSTM32Hal.h"

using namespace leka;

mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

LKCoreSTM32Hal hal;
CoreDACTimer coreTimer(hal);
CoreDAC coreDac(hal);

bool ended = false;

/**
 * @brief This function handles DMA1 stream5 global interrupt.
 */
extern "C" {
void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coreDac.getHandle().DMA_Handle1);
}
}

void fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t frequency, uint32_t samplingRate,
						   uint16_t maxValue, uint16_t minValue)
{
	uint32_t samplesPerPeriod = (samplingRate / frequency);

	for (uint32_t i = 0; i < bufferSize; ++i) {
		float tmp = 0.5 * sin(i * 2.0 * M_PI / samplesPerPeriod) + 0.5;
		tmp *= maxValue - minValue;
		buffer[i] = tmp + minValue;
	}
}

void callbackTest(DAC_HandleTypeDef *hdac)
{
	static uint32_t count	 = 0;
	static const int numSecs = 3;
	count++;
	if (count == (numSecs * 44100 / 256)) {
		coreTimer.stop();
		coreDac.stop();
		ended = true;
	}
}

auto main() -> int
{
	std::array<uint16_t, 512> outBuff;

	printf("\n\nHello, investigation day!\n\n");
	coreDac.setCptCallbackPtr(&callbackTest);
	coreDac.setHalfCptCallbackPtr(&callbackTest);
	// Init
	coreTimer.initialize(44100);
	coreDac.initialize();

	// generate data
	fillBufferWithSinWave(outBuff.data(), 512, 440, 44100, 0x999, 0x666);

	// start
	coreTimer.start();
	coreDac.start(outBuff.data(), 512);

	// wait preset period
	while (!ended) {
		rtos::ThisThread::sleep_for(50ms);
	}

	printf("End of audio test!\n\n");

	coreDac.deInitialize();
	coreTimer.deInitialize();
}