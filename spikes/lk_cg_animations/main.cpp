// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "AnimationBouncingSquare.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LKAnimationKit.h"
#include "LKCoreDMA2D.h"
#include "LKCoreDSI.h"
#include "LKCoreFatFs.h"
#include "LKCoreFont.h"
#include "LKCoreGraphics.h"
#include "LKCoreJPEG.h"
#include "LKCoreLCD.h"
#include "LKCoreLCDDriverOTM8009A.h"
#include "LKCoreLL.h"
#include "LKCoreLTDC.h"
#include "LKCoreSDRAM.h"
#include "LKCoreSTM32Hal.h"
#include "LKCoreVideo.h"
#include "SDBlockDevice.h"

using namespace leka;

HelloWorld hello;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
LKCoreFatFs corefatfs;

LKCoreLL corell;
CGPixel pixel(corell);
LKCoreSTM32Hal hal;
LKCoreSDRAM coresdram(hal);
LKCoreDMA2D coredma2d(hal);
LKCoreDSI coredsi(hal);
LKCoreLTDC coreltdc(hal, coredsi);
LKCoreGraphics coregraphics(coredma2d);
LKCoreFont corefont(pixel);
LKCoreLCDDriverOTM8009A coreotm(coredsi, PinName::SCREEN_BACKLIGHT_PWM);
LKCoreLCD corelcd(coreotm);
LKCoreJPEG corejpeg(hal, coredma2d, corefatfs);
LKCoreVideo corevideo(hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg);

Thread animation_thread;
AnimationBouncingSquare animation_bouncing_square(coregraphics);
LKAnimationKit animationkit(animation_thread, animation_bouncing_square);

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

int main(void)
{
	auto start = Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	corevideo.initialize();

	hello.start();

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);

		animationkit.start(animation_bouncing_square);
		rtos::ThisThread::sleep_for(5s);

		animationkit.stop();
		rtos::ThisThread::sleep_for(5s);
	}
}
