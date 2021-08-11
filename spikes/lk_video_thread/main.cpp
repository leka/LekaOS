#include "drivers/BufferedSerial.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreSDRAM.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LKCoreFatFs.h"
#include "LKCoreSTM32Hal.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

LKCoreSTM32Hal hal;
CoreSDRAM coresdram(hal);

VideoKit screen(hal);
VideoKit_DeclareIRQHandlers(screen);

rtos::Thread video_thread;
auto delay = HAL_GetTick(); 

void initializeSD()
{
	sd_blockdevice.init();
	sd_blockdevice.frequency(25'000'000);

	fatfs.mount(&sd_blockdevice);
}

void formatTime(char *buffer, int64_t time)
{
	int ds	= (time / 1000);
	int dms = (time / 1000.f - ds) * 100;
	sprintf(buffer, "%02d:%02d,%02d", ds / 60, ds % 60, dms);
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
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	initializeSD();

	coresdram.initialize();
	screen.initialize();
	
	// start video thread that plays a video
	video_thread.start(&videoThread);

	// do some work in main thread
	uint8_t buffer[1024];
	FileSystemKit::File file("/fs/assets/images/Leka/logo.jpg", "rb");

	while (file.is_open()) {
		auto start = HAL_GetTick();
		file.read(buffer, sizeof(buffer));
		delay = HAL_GetTick() - start;
		rtos::ThisThread::sleep_for(1ms);
	}

	video_thread.join();

	return 0;
}