// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/AnalogIn.h"
#include "drivers/DigitalIn.h"
#include "LogKit.h"
#include "events/EventQueue.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "include/IMUUtils.h"

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
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

#include "CoreBufferedSerial.h"
#include "CoreRFIDReader.h"
#include "RFIDKit.h"

#include "include/ODRCard.h"

using namespace std::literals::chrono_literals;
using namespace leka;



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
leka::LKCoreIMU coreimu(i2c, PinName::SENSOR_IMU_IRQ);

std::array<float, 3> xl_data;
std::array<float, 3> gy_data;


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
auto rfidreader = CoreRFIDReader(rfidserial);
auto rfidkit	= RFIDKit(rfidreader);	


/**************************** CARDS **************************/
ODRCard CARD_ODR_12_5HZ(0x0023, "ODR_IMU_12Hz", LSM6DSOX_XL_ODR_12Hz5, LSM6DSOX_GY_ODR_12Hz5, 80ms);
ODRCard CARD_ODR_26HZ(0x0024, "ODR_IMU_26Hz", LSM6DSOX_XL_ODR_26Hz, LSM6DSOX_GY_ODR_26Hz, 38ms);
ODRCard CARD_ODR_52HZ(0x0025, "ODR_IMU_52Hz", LSM6DSOX_XL_ODR_52Hz, LSM6DSOX_GY_ODR_52Hz, 19ms);
ODRCard CARD_ODR_104HZ(0x0027, "ODR_IMU_104Hz", LSM6DSOX_XL_ODR_104Hz, LSM6DSOX_GY_ODR_104Hz, 10ms);
std::array<ODRCard, 4> odr_cards = {CARD_ODR_12_5HZ, CARD_ODR_26HZ, CARD_ODR_52HZ, CARD_ODR_104HZ};


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

	coreimu.setDataRate(LSM6DSOX_XL_ODR_26Hz, LSM6DSOX_GY_ODR_26Hz);
	rtos::ThisThread::sleep_for(10ms);
	
		//by default
	auto current_tag_id = CARD_ODR_26HZ.getId();
	auto time_odr = CARD_ODR_26HZ.getTimeODR();
	videokit.displayImage(CARD_ODR_26HZ.getImgPath());

	rfidkit.init();
	rfidkit.onTagActivated([&id_card, &time_odr](const MagicCard &card) { 
		id_card = card.getId(); 
		for(ODRCard card : odr_cards)
		{
			if(card.getId() == id_card) {
				updateODR(videokit, coreimu, card.getImgPath(), card.getXlODR(), card.getGyODR(), time_odr, card.getTimeODR());
				break;
			}
		}
	});


	while (true) {
		auto start = rtos::Kernel::Clock::now();

		coreimu.getData(xl_data, gy_data);
			//mdps to dps
		gy_data[0] /= 1000;
		gy_data[1] /= 1000;
		gy_data[2] /= 1000;

		log_info("%f\t%f\t%f\t%f\t%f\t%f",xl_data[0], xl_data[1], xl_data[2], gy_data[0], gy_data[1], gy_data[2]);

		rtos::ThisThread::sleep_until(start + time_odr);
	}
}
