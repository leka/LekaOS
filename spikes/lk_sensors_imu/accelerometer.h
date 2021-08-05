#include "LSM6DSOX_ComponentAccelerometer.h"
#include "LSM6DSOX_ComponentGyroscope.h"

using namespace Component;

//##################################################################################################
// Global Variables
//##################################################################################################
I2C i2c1(PIN_I2C1_SDA, PIN_I2C1_SCL);
Communication::LSM6DSOX_I2C lsm6dsox_i2c(i2c1);

// DigitalInOut INT_1_LSM6DSOX(PIN_LSM6DSOX_INT1, PIN_OUTPUT, PullNone,
// 							0);	  // This line fix the use of LSM6DSOX on X-NUCLEO_IKS01A2
DigitalIn INT_1_LSM6DSOX(PIN_LSM6DSOX_INT1, PullNone);

// UnbufferedSerial serial(USBTX, USBRX);
//##################################################################################################
// Main
//##################################################################################################

auto main() -> int
{
	while (true) {
		ThisThread::sleep_for(500ms);
	}

	return 0;
}

/**
 * @brief Initialize the DataGatherer object
 * 	Set the data rate and range of the sensors as well as their power mode
 * 	Enable the block update parameter and attach the serial data receive IRQ
 *
 * @retval 0 in case of success, an error code otherwise
 */
auto init(Component::LSM6DSOX_Accelerometer &_accelerometer, Component::LSM6DSOX_Gyroscope &_gyroscope) -> Status
{
	// Initialize the component for driver usage
	if (_accelerometer.init() != Status::OK) {
		return Status::ERROR;
	}
	if (_gyroscope.init() != Status::OK) {
		return Status::ERROR;
	}

	//// Turn off Sensors
	if (_accelerometer.setPowerMode(Component::PowerMode::OFF) != Status::OK) {
		return Status::ERROR;
	}
	if (_gyroscope.setPowerMode(Component::PowerMode::OFF) != Status::OK) {
		return Status::ERROR;
	}

	// Enable Block Data Update
	if (lsm6dsox_block_data_update_set(&_register_io_function, PROPERTY_ENABLE) !=
		static_cast<int32_t>(Communication::Status::OK)) {
		return Status::ERROR;
	}

	// Configure accelerometer
	if (_accelerometer.setRange(AccelerometerRange::_2G) != Status::OK) return Status::ERROR;
	if (_accelerometer.setDataRate(104.0f) != Status::OK) return Status::ERROR;
	if (_accelerometer.setPowerMode(PowerMode::NORMAL) != Status::OK) return Status::ERROR;

	// Configure accelerometer
	if (_gyroscope.setRange(GyroscopeRange::_500DPS) != Status::OK) return Status::ERROR;
	if (_gyroscope.setDataRate(104.0f) != Status::OK) return Status::ERROR;
	if (_gyroscope.setPowerMode(PowerMode::NORMAL) != Status::OK) return Status::ERROR;

	// Serial config
	_serial.attach(callback(this, &LSM6DSOX_DataGatherer::onSerialReceived), UnbufferedSerial::RxIrq);
	//_serial.attach(_queue->event(_tickerCB), UnbufferedSerial::RxIrq);

	// Data rate and ticker config
	setDataRate(_dataRate);

	return Status::OK;
}

/**
 * @brief Disable I3C interface
 *
 * Disabling I3C on the sensor allows to use the INT1 pin as input while keeping I2C active.
 * If I3C is not disabled and INT1 pin stops being at 0, the sensor will shut down its I2C interface.
 *
 * @retval 0 in case of success, an error code otherwise
 */
auto disableI3C() -> Status
{
	/* Disable I3C interface */
	if (lsm6dsox_i3c_disable_set(&_register_io_function, LSM6DSOX_I3C_DISABLE) != (int32_t)Communication::Status::OK) {
		return Status::ERROR;
	}
	return Status::OK;
}

/**
 * @brief Get the data returned by the sensors
 *
 * @param data array in which to store the data from the sensors
 * @retval 0 in case of success, an error code otherwise
 */
Status getData(std::array<float, 6> &data)
{
	DataGathererData res;
	if (_accelerometer.getData(res.values.acc.data) != Status::OK) return Status::ERROR;
	if (_gyroscope.getData(res.values.gy.data) != Status::OK) return Status::ERROR;

	data = res.array;

	return Status::OK;
}

/**
 * @brief
 *
 * @param handle
 * @param write_address
 * @param p_buffer
 * @param number_bytes_to_write
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ptr_io_write(void *handle, uint8_t write_address, uint8_t *p_buffer, uint16_t number_bytes_to_write)
{
	return (int32_t)((LSM6DSOX_DataGatherer *)handle)
		->_lsm6dsox_component_i2c.write(write_address, number_bytes_to_write, p_buffer);
}

/**
 * @brief
 *
 * @param handle
 * @param read_address
 * @param p_buffer
 * @param number_bytes_to_read
 * @retval 0 in case of success, an error code otherwise
 */
int32_t ptr_io_read(void *handle, uint8_t read_address, uint8_t *p_buffer, uint16_t number_bytes_to_read)
{
	return (int32_t)((LSM6DSOX_DataGatherer *)handle)
		->_lsm6dsox_component_i2c.read(read_address, number_bytes_to_read, p_buffer);
}