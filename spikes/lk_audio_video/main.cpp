// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"
#include <string>

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "CoreAudio.h"
#include "CoreSDRAM.h"
#include "FATFileSystem.h"
#include "FileManager.h"
#include "LKCoreSTM32Hal.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;

LKCoreSTM32Hal hal;
CoreDAC coreDAC(hal);
CoreSDRAM coreSDRAM(hal);

VideoKit screen(hal);
VideoKit_DeclareIRQHandlers(screen);

rtos::Thread video_thread;

/**
 * @brief This function handles DMA1 stream5 global interrupt.
 */
extern "C" {
void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coreDAC.getHandle().DMA_Handle1);
}
}

void videoThread()
{
	gfx::Video video("/fs/assets/video/20fps_low10.avi");

	// play the video in a loop
	while (1) {
		screen.draw(video);

		video.nextFrame();

		if (video.hasEnded())
			video.restart();

		screen.display();
	}
}

auto main() -> int
{
	leka::FileManager sd_card;

	mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });
	log_info("\n\tHello, investigation day!");

	rtos::Thread audioThread;
	events::EventQueue audioEventQueue;

	CoreDACTimer coreDACTimer(hal, CoreDACTimer::HardWareBasicTimer::BasicTimer6);
	CoreAudio coreAudio(hal, coreDAC, coreDACTimer, audioThread, audioEventQueue);

	coreSDRAM.initialize();
	screen.initialize();

	// start video thread that plays a video
	video_thread.start(&videoThread);

	//video_thread.join();

	FIL file;
	std::string filename = "test-voix.wav";	  // sawtooth10_44ksamp_mono_5sec.wav

	while (true) {
		coreAudio.setVolume(30);
		
		filename = "test-voix-100Hzmin.wav";
		if (f_open(&file, filename.c_str(), FA_READ) == FR_OK) {
			log_info("\n\tFile %s did open!!", filename.c_str());
			coreAudio.playFile(&file);
		} else {
			log_info("\n\tCould not open file");
		}
		
		while (coreAudio.isPlaying()) {
			rtos::ThisThread::yield();
		}

		f_close(&file);

		
		coreAudio.setVolume(80);
		filename = "fur-elise-100Hzmin.wav";   // sawtooth10_44ksamp_mono_5sec.wav

		if (f_open(&file, filename.c_str(), FA_READ) == FR_OK) {
			log_info("\n\tFile %s did open!!", filename.c_str());
			coreAudio.playFile(&file);
		} else {
			log_info("\n\tCould not open file");
		}

		while (coreAudio.isPlaying()) {
			rtos::ThisThread::sleep_for(100ms);
		}

		f_close(&file);
		
		rtos::ThisThread::sleep_for(1s);
	}
}
