#include "mbed.h"

#include "DigitalOut.h"
#include "LSM6DSOX_CommunicationI2C.h"
#include "LSM6DSOX_ComponentAccelerometer.h"
#include "LSM6DSOX_ComponentGyroscope.h"
#include "LSM6DSOX_DataGatherer.h"

//##################################################################################################
// Defines
//##################################################################################################

#define PIN_I2C1_SDA	  (SENSOR_IMU_TH_I2C_SDA)
#define PIN_I2C1_SCL	  (SENSOR_IMU_TH_I2C_SCL)
#define PIN_LSM6DSOX_INT1 (SENSOR_IMU_IRQ)

//##################################################################################################
// Structs
//##################################################################################################

//##################################################################################################
// Prototypes
//##################################################################################################

void tickerCB();

//##################################################################################################
// Global Variables
//##################################################################################################
I2C i2c1(PIN_I2C1_SDA, PIN_I2C1_SCL);
Communication::LSM6DSOX_I2C lsm6dsox_i2c(i2c1);

// Component::LSM6DSOX_DataGatherer dataGatherer(lsm6dsox_i2c, PIN_LSM6DSOX_INT1, 10.0f, USBTX, USBRX,
// MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE, tickerCB);
Component::LSM6DSOX_DataGatherer dataGatherer(lsm6dsox_i2c, PIN_LSM6DSOX_INT1, 10.0f, USBTX, USBRX, 115200, tickerCB);

// DigitalInOut INT_1_LSM6DSOX(PIN_LSM6DSOX_INT1, PIN_OUTPUT, PullNone,
// 							0);	  // This line fix the use of LSM6DSOX on X-NUCLEO_IKS01A2

DigitalIn INT_1_LSM6DSOX(PIN_LSM6DSOX_INT1, PullNone);

// UnbufferedSerial serial(USBTX, USBRX);
//##################################################################################################
// Main
//##################################################################################################

auto main() -> int
{
	// Time for sensor init
	ThisThread::sleep_for(10ms);   // waiting for sensor startup

	// Greetings
	printf("  Starting a new run\n\n");

	// Restore default configuration
	dataGatherer.restoreDefaultConfiguration();

	// Disable I3C and setting INT1 pin as input
	// dataGatherer.disableI3C();
	// INT_1_LSM6DSOX.input();	  // setting up INT1 pin as input (for interrupts to work)

	// Components init
	dataGatherer.init();

	// Enable Block Data Update
	lsm6dsox_block_data_update_set(dataGatherer.TMP_getIoFunc(), PROPERTY_ENABLE);

	dataGatherer.setDataRate(26);

	printf("Start by configuring the data gathering. Then call \"start\" to start receiving data\n");
	printf("Write \"help\" for info on the functionnalities\n");
	printf(
		"If you are using mbed sterm don't forget to type \"CTRL + E\" to disable local echo and avoid corrupting the "
		"data\n");
	printf("\n\n");

	while (true) {
		dataGatherer.onTick();
		ThisThread::sleep_for(500ms);
	}

	return 0;
}

void tickerCB()
{
	// dataGatherer.onTick();
}