// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "AudioFrequency.h"
#include "AudioPlayer.h"
#include "FileManager.h"
#include "HelloWorld.h"

using namespace leka;

HelloWorld hello;
FileManager sd_card;

AnalogOut audio_output(MCU_SOUND_OUT);
DigitalOut audio_enable(SOUND_ENABLE, 1);

AudioFrequency audio_frequency(audio_output);
AudioPlayer player(&audio_output);

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello LekaOS, Audio test!\n\n");

	rtos::ThisThread::sleep_for(2s);

	// First, check haptic vibrator works
	audio_frequency.playFrequency(440, 1);	 // 440 Hz, 1s

	// Secondly, check that wav file exists
	const char filename[] = "assets/audio/file_example_WAV_1MG.wav";

	sd_card.open(filename);
	uint32_t file_size = sd_card.getFileSize();
	printf("File %s openened. File size : %lu bytes\n", filename, file_size);
	sd_card.close();

	// Finally, use AudioPlayer to play wav file on haptic vibrator
	File file(&sd_card._fs, filename);
	printf("Playng the song...\n");
	player.play(&file);
	printf("End of the song.\n");

	// hello.start();

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);
	}

	return 0;
}
