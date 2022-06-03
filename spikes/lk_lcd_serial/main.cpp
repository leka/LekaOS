// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// ? Run ./tools/display_image_via_serial.py

#include <string>

#include "events/EventQueue.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreEventFlags.h"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreJPEGModePolling.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLL.h"
#include "CoreLTDC.hpp"
#include "CoreSDRAM.hpp"
#include "CoreSTM32Hal.h"
#include "CoreVideo.hpp"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "SDBlockDevice.h"
#include "SerialCommunication.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

auto corell		   = CoreLL {};
auto pixel		   = CGPixel {corell};
auto hal		   = CoreSTM32Hal {};
auto coresdram	   = CoreSDRAM {hal};
auto coredma2d	   = CoreDMA2D {hal};
auto coredsi	   = CoreDSI {hal};
auto coreltdc	   = CoreLTDC {hal};
auto coregraphics  = CoreGraphics {coredma2d};
auto corefont	   = CoreFont {pixel};
auto coreotm	   = CoreLCDDriverOTM8009A {coredsi, PinName::SCREEN_BACKLIGHT_PWM};
auto corelcd	   = CoreLCD {coreotm};
auto _corejpegmode = CoreJPEGModeDMA {hal};
auto corejpeg	   = CoreJPEG {hal, _corejpegmode};
auto corevideo = CoreVideo {hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg};

HAL_VIDEO_DECLARE_IRQ_HANDLERS(corevideo);

auto event_flags = CoreEventFlags {};
auto videokit	 = VideoKit {event_flags, corevideo};

auto com_event_flags = rtos::EventFlags {};
auto thread			 = rtos::Thread {};
auto event_queue	 = events::EventQueue {};
auto communication	 = SerialCommunication {com_event_flags};

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

auto containsStartAndEndOfPath(auto image_path) -> bool
{
	auto containsStart	  = image_path.find("/fs") != std::string::npos;
	auto containsImageEnd = image_path.find(".jpg") != std::string::npos;
	auto containsVideoEnd = image_path.find(".avi") != std::string::npos;
	return containsStart && (containsImageEnd || containsVideoEnd);
}

auto isAnImage(auto path) -> bool
{
	auto containsImageFolder = path.find("img") != std::string::npos;
	auto containsImageEnd	 = path.find(".jpg") != std::string::npos;
	return containsImageFolder && containsImageEnd;
}

auto isAVideo(auto path) -> bool
{
	auto containsVideoFolder = path.find("vid") != std::string::npos;
	auto containsVideoEnd	 = path.find(".avi") != std::string::npos;
	return containsVideoFolder && containsVideoEnd;
}

auto main() -> int
{
	communication.sendMessage("Hello, World!\n");

	HelloWorld hello;
	hello.start();

	videokit.initializeScreen();

	initializeSD();

	corevideo.clearScreen();
	corelcd.setBrightness(1.0F);
	corelcd.turnOn();

	thread.start({&event_queue, &events::EventQueue::dispatch_forever});
	event_queue.call_every(50ms, &communication, &SerialCommunication::checkNewMessage);

	while (true) {
		com_event_flags.wait_any(NEW_MESSAGE);

		auto file_path = communication.getMessage();
		if (containsStartAndEndOfPath(file_path)) {
			if (isAnImage(file_path)) {
				videokit.displayImage(file_path);
				communication.sendMessage("ACK\n");
			} else if (isAVideo(file_path)) {
				videokit.playVideoOnce(file_path);
				communication.sendMessage("ACK\n");
			} else {
				communication.sendMessage("NOK\n");
			}
		} else {
			communication.sendMessage("NOK\n");
		}
	}
}
