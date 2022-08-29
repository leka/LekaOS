// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <cstdint>

#include <filesystem>
#include <memory>
#include "platform/Callback.h"

#include "drivers/AnalogIn.h"
#include "drivers/DigitalIn.h"
#include "events/EventQueue.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"


//RFID
#include "CoreBufferedSerial.h"
#include "CoreRFIDReader.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "RFIDKit.h"
#include "Ressources/class.h"

//LED
#include "include/register_all_average_and_raw_energy.h"
#include "include/IMUUtils.h"
#include "mbed.h"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "LedKit.h"
#include "EventLoopKit.h"
#include <string>
#include <type_traits>

//LCD SCREEN
#include "./Images.h"
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
#include "SDBlockDevice.h"
#include "VideoKit.h"


using namespace std::literals::chrono_literals;
using namespace leka;
using namespace std::chrono;



/*********************** LED animation ***********************/

namespace {

namespace leds {

	namespace internal {

		namespace ears {

			auto spi			= CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
			constexpr auto size = 2;

		}	// namespace ears

		namespace belt {

			auto spi			= CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
			constexpr auto size = 20;

		}	// namespace belt

	}	// namespace internal

	namespace animations {

		auto event_loop = EventLoopKit {};

	}	// namespace animations

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	void turnOff()
	{
		ears.setColor(RGB::black);
		belt.setColor(RGB::black);
		ears.show();
		belt.show();
	}

}	// namespace leds


auto hello	= HelloWorld {};
auto ledkit = LedKit {leds::animations::event_loop, leds::ears, leds::belt};

}	// namespace
/***********************************************************/


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

auto file = FileManagerKit::File {};

/*************************************************************/

// General tools
events::EventQueue event_queue;
rtos::Thread thread_action;
static uint8_t rst;

// IMU
auto const SIZE_SEQUENCE = 10;
mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
leka::LKCoreIMU coreimu(i2c, PinName::SENSOR_IMU_IRQ);

static uint8_t tx_buffer[1000];

lsm6dsox_odr_xl_t odr_xl = LSM6DSOX_XL_ODR_26Hz;
lsm6dsox_odr_g_t odr_g	 = LSM6DSOX_GY_ODR_26Hz;

stmdev_ctx_t dev_ctx;
lsm6dsox_emb_sens_t emb_sens;
lsm6dsox_all_sources_t status;
lsm6dsox_pin_int1_route_t pin_int1_route;
lsm6dsox_ctrl5_c_t ctrl5_c;
uint8_t mlc_out[8];

auto id_parcouru = 0;
auto name_curr_shape_c ="";
std::string name_curr_pic_shape ="";

auto desired_shape = "";
std::string desired_shape_pic = "";

/**************************** RFIDKit ************************/
auto rfidserial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
auto rfidreader = CoreRFIDReader(rfidserial);
auto rfidkit	= RFIDKit(rfidreader);

auto id_card = 0;
auto index_ = 0;
auto id_start_card = 0x0006;
/**************************** RFIDKit ************************/

// Game tools
auto game = false;
auto score = 0;
auto lives = 3;
auto interrupt_flag = false;


auto displayValidatedShape(auto index_) -> void{
	videokit.displayImage("/fs/home/img/shapesRecognition/Game/Shape_did_validated_plus.jpg");
	videokit.displayImage("/fs/home/img/shapesRecognition/Shapes/" + shapes[index_]->getPicName() + ".jpg");
	rtos::ThisThread::sleep_for(2s);
}

auto displayNotValidatedShape(auto name_curr_shape_) -> void{
	videokit.displayImage("/fs/home/img/shapesRecognition/Game/Shape_did_not_validated.jpg");
	videokit.displayImage("/fs/home/img/shapesRecognition/Shapes/" + name_curr_shape_ + ".jpg");
	rtos::ThisThread::sleep_for(2s);
}

auto displayScoreImage(auto score) -> void{
	switch(score){
		case 1:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/Score_one.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}
		case 2:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/Score_two.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}
		case 3:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/Score_three.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}
		case 4:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/Score_four.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}
		case 5:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/Score_five.jpg");
			rtos::ThisThread::sleep_for(2s);
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/congratulations.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}

	}
}

auto displayHeartsImage(auto lives) -> void {
	switch(lives){
		case 0:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/zero_hearts.jpg");
			rtos::ThisThread::sleep_for(2s);
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/game_over.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}
		case 1:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/one_hearts.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}
		case 2:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/two_hearts.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}
		case 3:{
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/three_hearts.jpg");
			rtos::ThisThread::sleep_for(2s);
			break;
		}
	}
}

/*
auto getIndexOfId(uint16_t id) -> int{
	log_debug("id = %d", id);
	auto id_parcouru = 0;
	for(size_t i=0; i<shapes.size(); ++i){
		id_parcouru = shapes[i]->getId();
		if(id == id_parcouru){
			log_debug("id_parcouru = %d", id_parcouru);
			log_debug("index : %d", i);
			return i;
		}
	}
	return -1;
}


auto checkMagicCard(auto ind, auto name) -> void
{
	log_debug("the card shape : %s", shapes[ind]->getNameChar());

	if(name == shapes[ind]->getNameChar())
		log_debug("Right shape");

	else
		log_debug("Wrong shape, try again");
}*/

void translate_array(std::array<std::uint8_t, 10> &array)
{
	for(int i = 0; i < 9; i++)
		array[i] = array[i+1];
}

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}


auto main() -> int
{

	auto start = rtos::Kernel::Clock::now();
	logger::init();

	leds::turnOff();

	log_info("Hello, World!\n\n");


	rtos::ThisThread::sleep_for(2s);

	videokit.initializeScreen();

	initializeSD();

	ledkit.init();

	for(size_t i=0; i<shapes.size(); ++i){
		id_parcouru = shapes[i]->getId();
		if(id_card == id_parcouru){
			//log_debug("id_parcouru = %d", id_parcouru);
			//log_debug("index : %d", i);
			index_ = i;
		}
	}

	rfidkit.init();
	rfidkit.onTagActivated([&id_card, &index_](const MagicCard &card){
		//index_ = 0;
		id_card = card.getId();
		log_debug("index_i : %d", index_);

		if(id_card == id_start_card){
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/select_shape_to_do.jpg");
		}
		else{
			interrupt_flag = true;
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/Shape_to_do.jpg");
			videokit.displayImage("/fs/home/img/shapesRecognition/Shapes/" + shapes[index_]->getPicName() + ".jpg");
		}
	});


	HelloWorld hello;
	hello.start();

	corevideo.clearScreen();
	rtos::ThisThread::sleep_for(1s);

	// Start Machine Learning Core configuration
	for (auto i = 0; i < (sizeof(test_) / sizeof(ucf_line_t)); i++) {
		coreimu.write(test_[i].address, 1, (uint8_t *)&test_[i].data);
	}

	coreimu.TurnOffEmbeddedFeatures(&emb_sens);
	rtos::ThisThread::sleep_for(10ms);

	coreimu.TurnOffSensors();
	coreimu.block_data_update_set(PROPERTY_ENABLE);

	coreimu.SetFullScale();
	coreimu.RouteSignalsInterruptGetSet(&pin_int1_route);
	coreimu.int_notification_set(LSM6DSOX_BASE_PULSED_EMB_LATCHED);
	// Enable embedded features
	coreimu.embedded_sens_set(&emb_sens);coreimu.data_rate_set(odr_xl, odr_g);

	coreimu.data_rate_set(odr_xl, odr_g);

	rtos::ThisThread::sleep_for(10ms);


	static constexpr int number_of_leds = 20;
	std::array<RGB, number_of_leds> expected_colors {};
	CoreSPI spi(LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK);
	CoreLED<number_of_leds> leds {spi};


	while(true) {
		id_card = 0;
		corelcd.setBrightness(0.9F);
		corelcd.turnOn();

		while(!game){
			videokit.displayImage("/fs/home/img/shapesRecognition/Game/accueuil.jpg");
			if(id_card == id_start_card)
				game = true;
		}

		while(game){
			for(size_t i=0; i<shapes.size(); ++i){
				id_parcouru = shapes[i]->getId();
				if(id_card == id_parcouru){
					index_ = i;
				}
			}
			desired_shape = shapes[index_]->getNameChar();
			desired_shape_pic = shapes[index_]->getPicName();
			coreimu.all_sources_get(&status);

			if(interrupt_flag){
				if (status.mlc1) {
					coreimu.mlc_out_get(mlc_out);
					sprintf((char *)tx_buffer, "Detect MLC interrupt code: %02X\r\n", mlc_out[0]);

					for(Interrupt inter : interrupts_array) {


						if(inter.getValue() == mlc_out[0])
						{
							if(inter.getValue() == Idle){
								log_debug("%s", inter.getNameChar());
								corevideo.clearScreen();
								videokit.displayImage("/fs/home/img/shapesRecognition/Game/Shape_to_do.jpg");
								videokit.displayImage("/fs/home/img/shapesRecognition/Shapes/" + desired_shape_pic + ".jpg");
							}
							else{
								corevideo.clearScreen();
								log_debug("%s", inter.getNameChar());
								videokit.displayImage("/fs/home/img/shapesRecognition/Segments/Segment_did.jpg");
								videokit.displayImage("/fs/home/img/shapesRecognition/Segments/" + inter.getImageName() + ".jpg");
							}
						}
					}


					// If Shape drawn
					translate_array(interrupts_sequence);
					interrupts_sequence.back() = mlc_out[0];

					for(AbstractField* shape: shapes){
						int indexCurrentInterrupt = shape->getIndexOfItem(mlc_out[0]);
						if(indexCurrentInterrupt != -1) {
							bool shapeRecognition = true;
							int index = indexCurrentInterrupt;
							int sizeSequence = shape->getSizeSequence();
							for(int i = 1; i < sizeSequence; ++i){
								index += 1;
								if(index == sizeSequence)
									index = 0;

								if(shape->getItemAtIndex(index) != interrupts_sequence[(SIZE_SEQUENCE-1)-sizeSequence+i]){
									shapeRecognition = false;
									break;
								}
							}

							if(shapeRecognition){
								interrupts_sequence = {0, 0, 0, 0, 0, 0, 0, 0 , 0, 0};
								name_curr_shape_c = shape->getNameChar();
								name_curr_pic_shape = shape->getPicName();
								//index_ = getIndexOfId(id_card);


								if(name_curr_shape_c == desired_shape){
									interrupt_flag = false;
									score++;
									ledkit.start(&LedKit::animation::rainbow);
									corevideo.clearScreen();
									displayValidatedShape(index_);
									displayScoreImage(score);
									id_card = 0; //reset id_card
									videokit.displayImage("/fs/home/img/shapesRecognition/Game/select_shape_to_do.jpg");
								}
								else{
									lives--;
									corevideo.clearScreen();
									displayNotValidatedShape(name_curr_pic_shape);
									displayHeartsImage(lives);
								}


								//End of the game
								if(score == 5 || lives == 0){
									game = false;
									score = 0;
									lives = 3;
								}
							}
						}
					}
				leds.show();
				}
			}
		}
		rtos::ThisThread::sleep_for(50ms);
		corelcd.turnOff();
	}
	rtos::ThisThread::sleep_for(100ms);
}

