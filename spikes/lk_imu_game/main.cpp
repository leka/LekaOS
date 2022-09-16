// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

/*
	This spike contains the Motion game whose purpose is to ask someone to execute a movement forming
	a specific shape while holding the robot.

	- At first, to start the game one needs to use RFID card with id 0X0006.
	- After that, one has to look at motion.h to get shapes id then choose with RFID card the shape to do.
	- The user try to execute the movement giving the right shape.
	- If he makes an other shape, he loose one life point, and if he makes the good one he wins a score point.
	- He has to form 5 different shapes to win the game, while being very careful because he has only 3 life points.
	- After the end of the game, it restarts.
*/

#include "drivers/AnalogIn.h"
#include "drivers/DigitalIn.h"
#include "events/EventQueue.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreBufferedSerial.h"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreEventFlags.h"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreIMULSM6DSOX.h"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreJPEGModePolling.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLED.h"
#include "CoreLL.h"
#include "CoreLTDC.hpp"
#include "CoreRFIDReaderCR95HF.h"
#include "CoreSDRAM.hpp"
#include "CoreSPI.h"
#include "CoreSTM32Hal.h"
#include "CoreVideo.hpp"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "MotionKit.h"
#include "RFIDKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"
#include "game.h"

using namespace std::literals::chrono_literals;
using namespace leka;
using namespace motion::game;

/**************************** LCD **************************/
SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
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
CoreIMULSM6DSOX coreimu(i2c, PinName::SENSOR_IMU_IRQ);

/**************************** FILE MANAGE **************************/
FATFileSystem fatfs("fs");
auto file = FileManagerKit::File {};

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

/**************************** RFID **************************/
auto rfidserial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
auto rfidreader = CoreRFIDReaderCR95HF(rfidserial);
auto rfidkit	= RFIDKit(rfidreader);

/**************************** LED **************************/
static constexpr int number_of_leds = 20;
std::array<RGB, number_of_leds> expected_colors {};
CoreSPI spi(LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK);
CoreLED<number_of_leds> leds {spi};

auto main() -> int
{
	auto id_card = 0;

	logger::init();
	videokit.initializeScreen();
	initializeSD();

	HelloWorld hello;
	hello.start();

	corevideo.clearScreen();
	rtos::ThisThread::sleep_for(2s);

	/********** IMU *********/
	coreimu.setMLCConfig<imu_register>(decision_tree_dir_diag);

	rtos::ThisThread::sleep_for(10ms);

	/****** RFID *******/
	rfidkit.init();
	rfidkit.onTagActivated([&](const MagicCard &card) {
		id_card = card.getId();
		log_debug("id card %0004X", id_card);

		if (id_card == card::start) {
			game_launch = true;
			rtos::ThisThread::sleep_for(10ms);
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/select_first.jpg");
		} else {
			interrupt_flag = true;
			auto counter   = 0;
			for (const motion::AbstractShape *shape: motion::shape::shapes_array) {
				if (shape->getId() == id_card) {
					index_ = counter;
					break;
				}
				counter++;
			}
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/Shape_to_do.jpg");
			videokit.displayImage("/fs/home/img/shapesRecognition/Shapes/" +
								  static_cast<std::string>(motion::shape::shapes_array.at(index_)->getImageName()) +
								  ".jpg");
		}
	});

	// MAIN
	while (true) {
		id_card = 0;
		corelcd.setBrightness(0.9F);
		corelcd.turnOn();
		leds.setColor(RGB::black);
		leds.show();

		if (!game_launch) {
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/accueuil.jpg");
			rtos::ThisThread::sleep_for(10ms);

		} else {
			coreimu.updateAllRessources();
			if (interrupt_flag) {
				desired_shape_pic = motion::shape::shapes_array.at(index_)->getImageName();

				if (coreimu.getStatus().mlc1) {
					auto output_tree = coreimu.getOutputTree(&coreimu.getRegisterIOFunction(), LSM6DSOX_MLC0_SRC);

					for (motion::tree::output::Segment inter: motion::tree::output::segments_array) {
						if (inter._value == output_tree) {
							if (output_tree == motion::tree::output::number::idle) {
								corevideo.clearScreen();
								videokit.displayImage("/fs/home/img/shapesRecognition/Game/Shape_to_do.jpg");
								videokit.displayImage("/fs/home/img/shapesRecognition/Shapes/" + desired_shape_pic +
													  ".jpg");
								break;
							}
							corevideo.clearScreen();
							videokit.displayImage("/fs/home/img/shapesRecognition/Game/Segment_did.jpg");
							videokit.displayImage("/fs/home/img/shapesRecognition/Segments/" +
												  static_cast<std::string>(inter._name_image) + ".jpg");

							::translate_array(::output_class_sequence);
							::output_class_sequence.back() = output_tree;

							for (const motion::AbstractShape *shape: motion::shape::shapes_array) {
								int indexCurrentInterrupt = shape->getIndexOfItem(output_tree);
								if (indexCurrentInterrupt != -1) {
									bool shapeRecognition	= true;
									int index				= indexCurrentInterrupt;
									const auto sizeSequence = static_cast<int>(shape->getSizeSequence());
									for (auto i = 1; i < sizeSequence; ++i) {
										index++;

										if (index == sizeSequence) {
											index = 0;
										}

										if (shape->getItemAtIndex(index) !=
											::output_class_sequence.at((::SEQUENCE_SIZE - 1) - sizeSequence + i)) {
											shapeRecognition = false;
											break;
										}
									}

									if (shapeRecognition) {
										output_class_sequence = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
										name_curr_pic_shape	  = shape->getImageName();

										if (name_curr_pic_shape == desired_shape_pic) {
											successShape(&leds, RGB::pure_green, &corevideo, videokit, id_card);
										} else {
											failedShape(&leds, RGB::pure_red, &corevideo, videokit);
										}

										// End of the game
										if (score == MAX_SCORE || lives == 0) {
											resetGame(id_card);
										}
									}
								}
							}
						}
					}
				}
			}
		}
		rtos::ThisThread::sleep_for(10ms);
	}
}
