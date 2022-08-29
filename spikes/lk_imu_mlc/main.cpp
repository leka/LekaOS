// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <filesystem>
#include <memory>
#include "platform/Callback.h"

#include "drivers/AnalogIn.h"
#include "drivers/DigitalIn.h"
#include "events/EventQueue.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "include/IMUUtils.h"
#include "include/lsm6dsox_reg.h"

#include "Ressources/decision_trees.h"

#include "mbed.h"
#include "CoreSPI.h"

//LCD SCREEN

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


#include "Ressources/class.h"

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
auto const SIZE_SEQUENCE = 10;
mbed::I2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
leka::LKCoreIMU coreimu(i2c, PinName::SENSOR_IMU_IRQ);

static uint8_t tx_buffer[1000];

lsm6dsox_odr_xl_t odr_xl = LSM6DSOX_XL_ODR_26Hz;
lsm6dsox_odr_g_t odr_g	 = LSM6DSOX_GY_ODR_26Hz;


/**************************** File Manager **************************/
auto file = FileManagerKit::File {};
void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

void translate_array(std::array<std::uint8_t, 10> &array)
{
	for(int i = 0; i < 9; i++)
		array[i] = array[i+1];
}

auto main() -> int
{

	stmdev_ctx_t dev_ctx;
	lsm6dsox_emb_sens_t emb_sens;
	lsm6dsox_all_sources_t status;
	lsm6dsox_pin_int1_route_t pin_int1_route;
	lsm6dsox_ctrl5_c_t ctrl5_c;
	uint8_t mlc_out[8];


	logger::init();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	videokit.initializeScreen();

	initializeSD();

	HelloWorld hello;
	hello.start();

	corevideo.clearScreen();
	rtos::ThisThread::sleep_for(1s);


		//IMU
	// Check the device ID
	if (coreimu.getId())	
		log_error("Fail to read device ID");
	else
		log_info("Device connected");


	// Start Machine Learning Core configuration */

	for (auto i = 0; i < (sizeof(mlc_configuration_final) / sizeof(ucf_line_t)); i++) {
		coreimu.WriteReg(&coreimu.get_reg_io_function(), mlc_configuration_final[i].address, (std::uint8_t *)&mlc_configuration_final[i].data, 1);
		rtos::ThisThread::sleep_for(10ms);
	}

	coreimu.TurnOffEmbeddedFeatures(&emb_sens);
	rtos::ThisThread::sleep_for(10ms);

	coreimu.TurnOffSensors();
	coreimu.setBlockDataUpdate(&coreimu.get_reg_io_function(), PROPERTY_ENABLE); 

	coreimu.SetFullScale();
	coreimu.RouteSignalsInterruptGetSet(&pin_int1_route);
	coreimu.setIntNotification(&coreimu.get_reg_io_function(), LSM6DSOX_BASE_PULSED_EMB_LATCHED);
	coreimu.setEmbeddedSens(&coreimu.get_reg_io_function(), &emb_sens);
	coreimu.setDataRate(odr_xl, odr_g);

	if (coreimu.getDataRate())
		log_info("Wrong ODR");
	else
		log_info("ODR de l'accelerometre et du mlc = 26Hz");


	coreimu.setMLCDataRate(&coreimu.get_reg_io_function(), LSM6DSOX_ODR_PRGS_26Hz);

	rtos::ThisThread::sleep_for(10ms);


	
	while(true) {
		corelcd.setBrightness(0.9F);
		corelcd.turnOn();

		coreimu.getAllRessources(&coreimu.get_reg_io_function(), &status);

		if (status.mlc1) {	
			coreimu.getMLCOut(&coreimu.get_reg_io_function(), mlc_out);
			sprintf((char *)tx_buffer, "Detect MLC interrupt code: %02X\r\n", mlc_out[0]);
			
			for(OutputClass inter : interrupts_array) {
				if(inter.getValue() == mlc_out[0])
				{
					log_debug("%s", inter.getNameChar());
					videokit.displayImage("/fs/home/img/shapesRecognition/Segments/" + inter.getImageName() + ".jpg");
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
						index++;
						if(index == sizeSequence)
							index = 0;
						
						if(shape->getItemAtIndex(index) != interrupts_sequence[(SIZE_SEQUENCE-1)-sizeSequence+i]){
							shapeRecognition = false;
							break;
						}
					}

					if(shapeRecognition) {
						interrupts_sequence = {0, 0, 0, 0, 0, 0, 0, 0 , 0, 0};
						videokit.displayImage("/fs/home/img/shapesRecognition/Shapes/" + shape->getImageName() + ".jpg");
						log_debug("Shape realised: %s", shape->getNameChar());
					}
				}
			}

		}

		rtos::ThisThread::sleep_for(50ms);
		corelcd.turnOff();
	}
}
