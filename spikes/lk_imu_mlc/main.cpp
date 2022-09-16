// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

/*
	This spike allows one to test the decision tree's results and performances.
	One has to check if all classes are recognized by the IMU LSM6DSOX and if all shapes
	contained in motion.h are well implemented.

	The Output Data Rate (ODR) of the Machine Learning Core (MLC) is defined
	at 26Hz by default.
*/

#include "mbed.h"
#include <filesystem>
#include <memory>

#include "drivers/AnalogIn.h"
#include "drivers/DigitalIn.h"
#include "events/EventQueue.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreIMULSM6DSOX.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "MotionKit.h"
#include "SDBlockDevice.h"

// LCD SCREEN

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
#include "FileManagerKit.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

/**************************** LCD **************************/
SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
auto event_flags = CoreEventFlags {};

CoreLL corell;
CGPixel pixel(corell);
CoreSTM32Hal hal;
CoreSDRAM coresdram(hal);
CoreDMA2D coredma2d(hal);
CoreDSI coredsi(hal);
CoreLTDC coreltdc(hal);
CoreGraphics coregraphics(coredma2d);
CoreFont corefont(pixel);
CoreLCDDriverOTM8009A coreotm(coredsi, PinName::SCREEN_BACKLIGHT_PWM);
CoreLCD corelcd(coreotm);
CoreJPEGModeDMA _corejpegmode {hal};
CoreJPEG corejpeg {hal, _corejpegmode};
CoreVideo corevideo(hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg);
HAL_VIDEO_DECLARE_IRQ_HANDLERS(corevideo);

auto videokit = VideoKit {event_flags, corevideo};

/**************************** IMU **************************/
mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
leka::CoreIMULSM6DSOX coreimu(i2c, PinName::SENSOR_IMU_IRQ);

/**************************** File Manager **************************/
auto file = FileManagerKit::File {};
void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	videokit.initializeScreen();

	initializeSD();

	HelloWorld hello;
	hello.start();

	corevideo.clearScreen();
	rtos::ThisThread::sleep_for(1s);

	// IMU

	// Start and set Machine Learning Core configuration
	coreimu.setMLCConfig<imu_register>(decision_tree_dir_diag);

	rtos::ThisThread::sleep_for(10ms);

	while (true) {
		corelcd.setBrightness(0.9f);
		corelcd.turnOn();

		coreimu.updateAllRessources();

		if (coreimu.getStatus().mlc1) {
			auto output_tree = coreimu.getOutputTree(&coreimu.getRegisterIOFunction(), LSM6DSOX_MLC0_SRC);

			for (motion::tree::output::Segment inter: motion::tree::output::segments_array) {
				if (inter._value == output_tree) {
					videokit.displayImage("/fs/home/img/shapesRecognition/Segments/" +
										  static_cast<std::string>(inter._name_image) + ".jpg");
				}
			}

			// If Shape drawn
			motion::game::translate_array(motion::game::output_class_sequence);
			motion::game::output_class_sequence.back() = output_tree;

			for (const motion::AbstractShape *shape: motion::shape::shapes_array) {
				int indexCurrentInterrupt = shape->getIndexOfItem(output_tree);
				if (indexCurrentInterrupt != -1) {
					auto shapeRecognition	= true;
					auto index				= indexCurrentInterrupt;
					const auto sizeSequence = static_cast<int>(shape->getSizeSequence());
					for (auto i = 1; i < sizeSequence; ++i) {
						index++;
						if (index == sizeSequence) {
							index = 0;
						}

						if (shape->getItemAtIndex(index) != motion::game::output_class_sequence.at(
																(motion::game::SEQUENCE_SIZE - 1) - sizeSequence + i)) {
							shapeRecognition = false;
							break;
						}
					}

					if (shapeRecognition) {
						motion::game::output_class_sequence = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
						videokit.displayImage("/fs/home/img/shapesRecognition/Shapes/" +
											  static_cast<std::string>(shape->getImageName()) + ".jpg");
					}
				}
			}
		}

		rtos::ThisThread::sleep_for(50ms);
		corelcd.turnOff();
	}
}
