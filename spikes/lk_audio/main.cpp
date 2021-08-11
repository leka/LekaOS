// Leka - LekaOS
// Copyright 2021 APF France handicap
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
CoreDACTimer coreDACTimer(hal, CoreDACTimer::HardWareBasicTimer::BasicTimer6);
CoreDAC coreDAC(hal);
CoreAudio coreAudio(hal, coreDAC, coreDACTimer, audioThread, audioEventQueue);

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
	printf("\n\nHello, investigation day!\n\n");

	FIL file;
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

		while (coreAudio.isPlaying()) {
			rtos::ThisThread::sleep_for(50ms);
		}

		f_close(&file);

		coreAudio.setVolume(80);
		filename = "fur-elise-100Hzmin.wav";   // sawtooth10_44ksamp_mono_5sec.wav

		if (f_open(&file, filename.c_str(), FA_READ) == FR_OK) {
			printf("\n\nFile %s did open!!\n", filename.c_str());
			coreAudio.playFile(&file);
		} else {
			printf("Could not open file\n");
		}

		while (coreAudio.isPlaying()) {
			rtos::ThisThread::sleep_for(50ms);
		}

		f_close(&file);
	}
}
