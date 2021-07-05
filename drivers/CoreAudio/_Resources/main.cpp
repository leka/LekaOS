// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WavePlayer.h"

mbed::DigitalOut audio_enable(SOUND_ENABLE, 0);

leka::FileManager sd_card;

static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

auto main() -> int
{
	printf("\nHello, Investigation Day!\n\n");

	while (true) {
		//string filename = "fur-elise.wav";
		string filename = "sawtooth10_44ksamp_mono_5sec.wav";

		PlayWavFile(filename);

		rtos::ThisThread::sleep_for(2s);
	}
}
