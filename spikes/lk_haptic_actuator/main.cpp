// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"
#include <string>

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "CoreAudio.h"
#include "FATFileSystem.h"
#include "FileManager.h"
#include "LKCoreSTM32Hal.h"
#include "SDBlockDevice.h"

using namespace leka;

leka::FileManager sd_card;

mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

rtos::Thread audioThread;
events::EventQueue audioEventQueue;

LKCoreSTM32Hal hal;
CoreDACTimer coreDACTimer(hal);
CoreDAC coreDAC(hal);
CoreAudio coreAudio(hal, coreDAC, coreDACTimer, audioThread, audioEventQueue);

// Initialise the digital pin LED1 as an output
mbed::DigitalOut led(LED1);

FIL file;

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
	led.write(0);
	printf("\n\nHello, on a mis autre chose!\n\n");

	std::string filename = "test-voix.wav";	  // sawtooth10_44ksamp_mono_5sec.wav

	while (true) {
		coreAudio.setVolume(30);
		filename = "test-voix-100Hzmin.wav";
		if (f_open(&file, filename.c_str(), FA_READ) == FR_OK) {
			printf("\n\nFile %s did open!!\n", filename.c_str());
			coreAudio.playFile(&file);
		} else {
			printf("Could not open file\n");
		}
		int i = 0;
		printf("playing : %d\n", static_cast<int>(coreAudio.playing));
		while (coreAudio.playing) {
			// printf("waiting\n");
			// rtos::ThisThread::sleep_for(50ms);
			i++;
		}
		printf("playing should be zero : %d\n", static_cast<int>(coreAudio.playing));

		f_close(&file);

		coreAudio.setVolume(80);
		filename = "fur-elise-100Hzmin.wav";   // sawtooth10_44ksamp_mono_5sec.wav

		if (f_open(&file, filename.c_str(), FA_READ) == FR_OK) {
			printf("\n\nFile %s did open!!\n", filename.c_str());
			coreAudio.playFile(&file);
		} else {
			printf("Could not open file\n");
		}

		while (coreAudio.playing) {
			rtos::ThisThread::sleep_for(50ms);
		}

		f_close(&file);
	}
}
