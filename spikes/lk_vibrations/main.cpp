// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

#include "CoreAudio.h"
#include "CoreVibration.h"
#include "HelloWorld.h"
#include "LKCoreSTM32Hal.h"
//#include "VibrationsUtils.h"

using namespace leka;
using namespace std::chrono;

// uint16_t _outBuffer[OUT_BUFF_SIZE] = {0};

mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

LKCoreSTM32Hal hal;
CoreDACTimer coreDACTimer(hal);
CoreDAC coreDAC(hal);

rtos::Thread vibrationsThread;
events::EventQueue vibrationsEventQueue;

CoreVibration coreVib(hal, coreDAC, coreDACTimer, vibrationsThread, vibrationsEventQueue);

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
	coreVib.initialize(2048);

	float duration	   = 2.0;
	uint32_t frequency = 128;
	float amplitude	   = 0.1;

	VibrationTemplate vib1(duration, frequency, amplitude, false);
	VibrationTemplate vib2(1, 200, 0.3, false);
	VibrationTemplate vib3(0.5, 128, 0.1, false);
	VibrationTemplate pause_1s(1, 1, 0, false);
	VibrationTemplate pause_2s(2, 1, 0, false);
	VibrationTemplate pause_500ms(0.5, 1, 0, false);

	vector<VibrationTemplate> sequence;
	sequence.push_back(vib1);
	sequence.push_back(pause_2s);

	sequence.push_back(vib2);
	sequence.push_back(pause_1s);
	sequence.push_back(vib2);
	sequence.push_back(pause_1s);

	sequence.push_back(vib3);
	sequence.push_back(pause_500ms);
	sequence.push_back(vib3);
	sequence.push_back(pause_500ms);
	sequence.push_back(vib3);
	sequence.push_back(pause_500ms);
	sequence.push_back(vib3);
	sequence.push_back(pause_500ms);

	while (true) {
		printf("Playing vibrations !\n");
		coreVib.playVibrationSequence(sequence);
		rtos::ThisThread::sleep_for(1s);
	}

	coreVib.deInit();
}