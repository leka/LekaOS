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

#include "rtos/ThisThread.h"

#include "CoreBufferedSerial.h"
#include "CoreIMULSM6DSOX.h"
#include "CoreLED.h"
#include "CoreRFIDReaderCR95HF.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "ODRCard.h"
#include "RFIDKit.h"

using namespace std::literals::chrono_literals;
using namespace leka;
using namespace motion::record;

namespace {

namespace leds {

	namespace internal::belt {

		auto spi			= CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
		constexpr auto size = 20;

	}	// namespace internal::belt

	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

}	// namespace leds

namespace imu {

	mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	leka::CoreIMULSM6DSOX coreimu(i2c);

}	// namespace imu

namespace rfid {

	auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
	auto reader = CoreRFIDReaderCR95HF(serial);

}	// namespace rfid

auto rfidkit = RFIDKit(rfid::reader);

namespace cards::odr {

	ODRCard CARD_ODR_12_5HZ(card::ODR_12_5HZ, RGB::pure_red, LSM6DSOX_XL_ODR_12Hz5, LSM6DSOX_GY_ODR_12Hz5, 80ms);
	ODRCard CARD_ODR_26HZ(card::ODR_26HZ, RGB::pure_green, LSM6DSOX_XL_ODR_26Hz, LSM6DSOX_GY_ODR_26Hz, 38ms);
	ODRCard CARD_ODR_52HZ(card::ODR_52HZ, RGB::pure_blue, LSM6DSOX_XL_ODR_52Hz, LSM6DSOX_GY_ODR_52Hz, 19ms);
	ODRCard CARD_ODR_104HZ(card::ODR_104HZ, RGB::purple, LSM6DSOX_XL_ODR_104Hz, LSM6DSOX_GY_ODR_104Hz, 10ms);
	std::array<ODRCard, 4> odr_cards = {CARD_ODR_12_5HZ, CARD_ODR_26HZ, CARD_ODR_52HZ, CARD_ODR_104HZ};
}	// namespace cards::odr

// Default configuration
auto current_tag_id = cards::odr::CARD_ODR_26HZ.getId();
auto time_odr		= cards::odr::CARD_ODR_26HZ.getTimeODR();

}	// namespace

auto main() -> int
{
	auto id_card = 0;

	logger::init();

	HelloWorld hello;
	hello.start();

	rfidkit.init();
	rfidkit.onTagActivated([&id_card](const MagicCard &card) {
		id_card = card.getId();
		for (ODRCard odr_card: cards::odr::odr_cards) {
			if (odr_card.getId() == id_card) {
				updateODR<leka::CoreIMULSM6DSOX::odr_xl_t, leka::CoreIMULSM6DSOX::odr_g_t>(
					leds::belt, imu::coreimu, odr_card.getColor(), odr_card.getXlODR(), odr_card.getGyODR(), time_odr,
					odr_card.getTimeODR());
				break;
			}
		}
	});

	while (true) {
		auto start = rtos::Kernel::Clock::now();
		std::tuple<interface::mg_t, interface::mg_t, interface::mg_t, interface::dps_t, interface::dps_t,
				   interface::dps_t>
			imu_data;
		imu_data = imu::coreimu.getAccelAndAngularSpeed();

		log_info("Acc_x(mg): %f, Acc_y(mg): %f, Acc_z(mg): %f, Gyr_x(dps): %f, Gyr_y(dps): %f, Gyr_z(dps): %f)",
				 std::get<0>(imu_data), std::get<1>(imu_data), std::get<2>(imu_data), std::get<3>(imu_data),
				 std::get<4>(imu_data), std::get<5>(imu_data));
		rtos::ThisThread::sleep_until(start + time_odr);
	}
}
