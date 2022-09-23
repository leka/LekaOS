// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

/*
	This spike sends IMU LSM6DSOX's data of : accelerometer (axis X, Y and Z) in mg
	and gyroscope (axis X,Y and Z) in dps to the serial.
	By default, the sending frequency is 26Hz, corresponding to 38ms.
	This frequency is easily editable with RFID Cards where one can select
	12.5Hz, 26Hz, 52Hz and 104Hz. One can check the ODRCard.h to get corresponding card's id.

	This spike must be launched if one want to record data logs with the application
	Record_IMU_App developped in python.
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
#include "CoreLL.h"
#include "CoreLTDC.hpp"
#include "CoreRFIDReaderCR95HF.h"
#include "CoreSDRAM.hpp"
#include "CoreSTM32Hal.h"
#include "CoreVideo.hpp"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "ODRCard.h"
#include "RFIDKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace std::literals::chrono_literals;
using namespace leka;
using namespace motion::record;

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
leka::CoreIMULSM6DSOX coreimu(i2c, PinName::SENSOR_IMU_IRQ);

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

/**************************** CARDS **************************/
ODRCard CARD_ODR_12_5HZ(card::ODR_12_5HZ, "ODR_IMU_12Hz", LSM6DSOX_XL_ODR_12Hz5, LSM6DSOX_GY_ODR_12Hz5, 80ms);
ODRCard CARD_ODR_26HZ(card::ODR_26HZ, "ODR_IMU_26Hz", LSM6DSOX_XL_ODR_26Hz, LSM6DSOX_GY_ODR_26Hz, 38ms);
ODRCard CARD_ODR_52HZ(card::ODR_52HZ, "ODR_IMU_52Hz", LSM6DSOX_XL_ODR_52Hz, LSM6DSOX_GY_ODR_52Hz, 19ms);
ODRCard CARD_ODR_104HZ(card::ODR_104HZ, "ODR_IMU_104Hz", LSM6DSOX_XL_ODR_104Hz, LSM6DSOX_GY_ODR_104Hz, 10ms);
std::array<ODRCard, 4> odr_cards = {CARD_ODR_12_5HZ, CARD_ODR_26HZ, CARD_ODR_52HZ, CARD_ODR_104HZ};

// by default
auto current_tag_id = CARD_ODR_26HZ.getId();
auto time_odr		= CARD_ODR_26HZ.getTimeODR();

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

	videokit.displayImage(CARD_ODR_26HZ.getImgPath());

	rfidkit.init();
	rfidkit.onTagActivated([&](const MagicCard &card) {
		id_card = card.getId();
		for (ODRCard card: odr_cards) {
			if (card.getId() == id_card) {
				updateODR<leka::CoreIMULSM6DSOX::odr_xl_t, leka::CoreIMULSM6DSOX::odr_g_t>(
					videokit, coreimu, card.getImgPath(), card.getXlODR(), card.getGyODR(), time_odr,
					card.getTimeODR());
				break;
			}
		}
	});

	while (true) {
		auto start = rtos::Kernel::Clock::now();
		std::tuple<interface::mg_t, interface::mg_t, interface::mg_t, interface::dps_t, interface::dps_t,
				   interface::dps_t>
			imu_data;
		imu_data = coreimu.getAccelAndAngularSpeed();

		log_info("%f\t%f\t%f\t%f\t%f\t%f", std::get<0>(imu_data), std::get<1>(imu_data), std::get<2>(imu_data),
				 std::get<3>(imu_data), std::get<4>(imu_data), std::get<5>(imu_data));
		rtos::ThisThread::sleep_until(start + time_odr);
	}
}
