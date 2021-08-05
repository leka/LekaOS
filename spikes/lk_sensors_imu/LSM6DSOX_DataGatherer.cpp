#include "LSM6DSOX_DataGatherer.h"

namespace Component {

/**
 * @brief Construct a new LSM6DSOX_DataGatherer object
 *
 * @param component_i2c I2C communication interface
 * @param pin_interrupt1 Device pin to which the LSM6DSOX INT1 pin is connected
 * @param dataRate Rate of the datagathering
 * @param tx Serial transmission pin
 * @param rx Serial receive ping
 * @param baud baudrate of the serial communication
 * @param callBack Callback function to be called periodically by the ticker
 */
LSM6DSOX_DataGatherer::LSM6DSOX_DataGatherer(Communication::I2CBase &component_i2c, PinName pin_interrupt1,
											 float dataRate, PinName tx, PinName rx, int baud, void (*callBack)())
	: _lsm6dsox_component_i2c(component_i2c),
	  _mcu_pin_interrupt1(pin_interrupt1),
	  _lsm6dsox_interrupt1(pin_interrupt1),
	  _accelerometer(component_i2c, pin_interrupt1),
	  _gyroscope(component_i2c, pin_interrupt1),
	  _dataRate(dataRate),
	  _serial(tx, rx, baud),
	  _tickerCB(callBack)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;

	_queue = mbed_event_queue();
	//_serialHandler = _queue->event(this, &LSM6DSOX_DataGatherer::client_sigio, client);

	_serialInputStr.reserve(100);
}

/**
 * @brief Initialize the DataGatherer object
 * 	Set the data rate and range of the sensors as well as their power mode
 * 	Enable the block update parameter and attach the serial data receive IRQ
 *
 * @retval 0 in case of success, an error code otherwise
 */
auto LSM6DSOX_DataGatherer::init() -> Status
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
auto LSM6DSOX_DataGatherer::disableI3C() -> Status
{
	/* Disable I3C interface */
	if (lsm6dsox_i3c_disable_set(&_register_io_function, LSM6DSOX_I3C_DISABLE) != (int32_t)Communication::Status::OK) {
		return Status::ERROR;
	}
	return Status::OK;
}

/**
 * @brief Get the ID of the LSM6DSOX sensor connected
 *
 * @param id where to store the ID
 * @retval 0 in case of success, an error code otherwise
 */
auto LSM6DSOX_DataGatherer::getID(uint8_t &id) -> Status
{
	uint8_t p_data = 0;
	if (lsm6dsox_device_id_get(&_register_io_function, &p_data) != (int32_t)Communication::Status::OK) {
		return Status::ERROR;
	}
	id = p_data;

	return Status::OK;
}

/**
 * @brief Restores the default values of all the control registers of the sensor
 *
 * @retval 0 in case of success, an error code otherwise
 */
Status LSM6DSOX_DataGatherer::restoreDefaultConfiguration()
{
	uint8_t rst;
	if (lsm6dsox_reset_set(&_register_io_function, PROPERTY_ENABLE) != (int32_t)Communication::Status::OK) {
		return Status::ERROR;
	}
	do {
		if (lsm6dsox_reset_get(&_register_io_function, &rst) != (int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
	} while (rst);

	return Status::OK;
}

// this is a temporary method to ease up developpement
// TODO erase once no more in use
/**
 * @brief Allow to recover the Register IO function from the inner class
 *
 * @retval 0 in case of success, an error code otherwise
 */
stmdev_ctx_t *LSM6DSOX_DataGatherer::TMP_getIoFunc()
{
	return &_register_io_function;
}

/**
 * @brief Set the data rate of the data gathering
 *
 * @param dataRate data rate to set to the data gathering process
 */
void LSM6DSOX_DataGatherer::setDataRate(float dataRate)
{
	if (dataRate < 0)
		dataRate = 0.0f;
	else if (dataRate > 104.0f)
		dataRate = 104.0f;
	_dataRate = dataRate;
}

/**
 * @brief Set the data rate of the data gathering while the communication process is running
 *
 * @param dataRate data rate to set to the data gathering process
 */
void LSM6DSOX_DataGatherer::setDataRateWhileRunning(float dataRate)
{
	setDataRate(dataRate);
	stopTicker();
	startTicker();
}

/**
 * @brief Get the data rate of the data gathering
 *
 * @param data_rate data rate of the data gathering process
 */
void LSM6DSOX_DataGatherer::getDataRate(float &data_rate)
{
	data_rate = _dataRate;
}

/**
 * @brief Get the data returned by the sensors
 *
 * @param data array in which to store the data from the sensors
 * @retval 0 in case of success, an error code otherwise
 */
Status LSM6DSOX_DataGatherer::getData(std::array<float, 6> &data)
{
	DataGathererData res;
	if (_accelerometer.getData(res.values.acc.data) != Status::OK) return Status::ERROR;
	if (_gyroscope.getData(res.values.gy.data) != Status::OK) return Status::ERROR;

	data = res.array;

	return Status::OK;
}

/**
 * @brief Get the interrupt pin currently used as the interrupt pin
 *
 * @return PinName name of the pin used as interrupt pin
 */
PinName LSM6DSOX_DataGatherer::getInterruptPin()
{
	return _mcu_pin_interrupt1;
}

/**
 * @brief Enable all interrupts on the LSM6DSOX (Should be moved elsewhere)
 *
 * @retval 0 in case of success, an error code otherwise
 */
Status LSM6DSOX_DataGatherer::enableInterrupt()
{
	_lsm6dsox_interrupt1.enable_irq();
	return Status::OK;
}

/**
 * @brief Disable all interrupts on the LSM6DSOX (Should be moved elsewhere)
 *
 * @retval 0 in case of success, an error code otherwise
 */
Status LSM6DSOX_DataGatherer::disableInterrupt()
{
	_lsm6dsox_interrupt1.disable_irq();
	return Status::OK;
}

/**
 * @brief
 *
 */
void LSM6DSOX_DataGatherer::printHeader()
{
	string msg = "\n\nSTEVAL-MKI197V1 (LSM6DSOX)\n\n";
	_serial.write(msg.c_str(), msg.size());

	msg = "A_X [mg]\tA_Y [mg]\tA_Z [mg]\tG_X [mg]\tG_Y [mg]\tG_Z [mg]\n";
	_serial.write(msg.c_str(), msg.size());
}

/**
 * @brief Print the gathered data to the serial
 *
 * @param data gathered to send via serial
 */
void LSM6DSOX_DataGatherer::printData(DataGathererData data)
{
	char nb[30];
	string msg = "";

	for (int i = 0; i < 6; ++i) {
		sprintf(nb, "%d\t", (int)(data.array[i]));
		msg.append(nb);
	}
	msg.append("\n");

	_serial.write(msg.c_str(), msg.size());
}

/**
 * @brief IRQ when a serial is received
 * Store the command in progress until it's complete then parse the command
 */
void LSM6DSOX_DataGatherer::onSerialReceived()
{
	char chr = '\0';
	while (_serial.readable()) {
		_serial.read(&chr, 1);
		if (chr != '\n' && chr != '\0' && chr != '\r')
			_serialInputStr.push_back(chr);
		else if (chr != '\r')
			_queue->call(callback(this, &LSM6DSOX_DataGatherer::parseCommand));
	}
}

/**
 * @brief Send the data as a binary to reduce sending time
 *
 * @param var : data to send as binary
 */
void LSM6DSOX_DataGatherer::sendIntBinary(int var)
{
	char *bytePtr = (char *)&var;
	for (int i = 3; i >= 0; --i) {
		_serial.write((void *)(bytePtr + i), 1);
	}
}

/**
 * @brief Send the data in binary when the ticker ticks
 *
 */
void LSM6DSOX_DataGatherer::onTick()
{
	DataGathererData d;
	getData(d.array);

	// to send data in binary format
	// for(int i = 0 ; i < 6; i++){
	// 	sendIntBinary((int)d.array[i]);
	// }
	// to send data via a printf (slower)
	printData(d);
}

/**
 * @brief Start the ticker at the set data rate
 *
 */
void LSM6DSOX_DataGatherer::startTicker()
{
	stopTicker();
	std::chrono::microseconds period_us((long)(1000000 / _dataRate));
	_ticker.attach(_queue->event(_tickerCB), period_us);
}

/**
 * @brief Stop the ticker
 *
 */
void LSM6DSOX_DataGatherer::stopTicker()
{
	_ticker.detach();
}

/**
 * @brief Parse the commands received in serial in order to control the data gathering process
 * 	See the README.md file in the section "using the code" for the commands details
 *
 */
void LSM6DSOX_DataGatherer::parseCommand()
{
	// TODO change commandstring
	string commandString = _serialInputStr;

	string msg;
	char chars[100];
	int nbChars;

	msg = "Your command is: \"" + commandString + "\"\n";
	_serial.write(msg.c_str(), msg.size());

	if (commandString == "start") {
		printHeader();
		startTicker();
	} else if (commandString == "stop") {
		stopTicker();
	} else if (commandString.substr(0, 5) == "rate ") {
		commandString = commandString.substr(5, commandString.length() - 5);
		msg			  = "Rate you want to set: " + commandString + "\n";
		_serial.write(msg.c_str(), msg.size());

		float rate;
		if (isFloat(commandString)) {
			rate = stof(commandString);
			if (rate > 0.0 && rate <= 104.0) {
				setDataRate(rate);

				nbChars = sprintf(chars, "New data rate: %d\n\n", (int)rate);
				_serial.write(chars, nbChars);
			} else {
				nbChars = sprintf(chars, "Data rate out of bounds, data rate still set to : '%d'\n", (int)_dataRate);
				_serial.write(chars, nbChars);
			}
		} else {
			nbChars = sprintf(chars, "Data rate not valid, data rate still set to : '%d'\n", (int)_dataRate);
			_serial.write(chars, nbChars);
		}
	} else if (commandString.substr(0, 6) == "range ") {
		msg = "Changing range\n";
		_serial.write(msg.c_str(), msg.size());

		commandString = commandString.substr(6, commandString.length() - 6);
		if (commandString.substr(0, 3) == "xl ") {
			msg = "On accelerometer\n";
			_serial.write(msg.c_str(), msg.size());

			commandString = commandString.substr(3, commandString.length() - 3);
			Component::AccelerometerRange range;
			_accelerometer.getRange(range);

			if (commandString == "2G")
				range = Component::AccelerometerRange::_2G;
			else if (commandString == "4G")
				range = Component::AccelerometerRange::_4G;
			else if (commandString == "8G")
				range = Component::AccelerometerRange::_8G;
			else if (commandString == "16G")
				range = Component::AccelerometerRange::_16G;
			else {
				nbChars = sprintf(chars, "Invalid range input, range is still %dG\n", (int)range);
				_serial.write(chars, nbChars);
			}

			_accelerometer.setRange(range);

			nbChars = sprintf(chars, "Accelerometer range set to: %dG\n", (int)range);
			_serial.write(chars, nbChars);

		} else if (commandString.substr(0, 3) == "gy ") {
			msg = "On gyroscope\n";
			_serial.write(msg.c_str(), msg.size());

			commandString = commandString.substr(3, commandString.length() - 3);
			Component::GyroscopeRange range;
			_gyroscope.getRange(range);

			if (commandString == "125DPS")
				range = Component::GyroscopeRange::_125DPS;
			else if (commandString == "250DPS")
				range = Component::GyroscopeRange::_250DPS;
			else if (commandString == "500DPS")
				range = Component::GyroscopeRange::_500DPS;
			else if (commandString == "1000DPS")
				range = Component::GyroscopeRange::_1000DPS;
			else if (commandString == "2000DPS")
				range = Component::GyroscopeRange::_2000DPS;
			else {
				nbChars = sprintf(chars, "Invalid range input, range is still %dDPS\n", (int)range);
				_serial.write(chars, nbChars);
			}

			_gyroscope.setRange(range);

			nbChars = sprintf(chars, "Gyroscope range set to: %dDPS\n", (int)range);
			_serial.write(chars, nbChars);
		} else {
			msg = "Not a valid device chosen (xl or gy)\n";
			_serial.write(msg.c_str(), msg.size());
		}
	} else if (commandString == "help") {
		stopTicker();
		msg = "List of commands : \n";
		msg += "- start : start sending data\n";
		msg += "- stop : stop sending data\n";
		msg += "- rate [value] : change data rate\n";
		msg += "- range [device] [value] : change device range (full scale)\n";
		msg += "\tdevice : \"xl\" or \"gy\" \n";
		msg += "\tvalues : same as in driver \n";
		msg += "\t\t \"xG\" for accelerometer \n";
		msg += "\t\t \"xDPS\" for gyroscope \n";
		msg += "\t\t \"xG\" for accelerometer \n";
		_serial.write(msg.c_str(), msg.size());

	} else {
		msg = "Invalid command, type 'help' for more info\n";
		_serial.write(msg.c_str(), msg.size());
	}

	_serialStrComplete = false;
	_serialInputStr	   = "";
}

/**
 * @brief Test if a string is a float number or not
 *
 * @param s string to test as a float
 * @return true the string is a float
 * @return false the string is not a float
 */
bool LSM6DSOX_DataGatherer::isFloat(string s)
{
	int numPoints = 0;
	for (unsigned int i = 0; i < s.length(); ++i) {
		if (s[i] == '.') {
			if ((numPoints == 0) && (i != 0) && (i != s.length() - 1))
				numPoints++;
			else
				return false;
		} else if ((s[i] < '0') || (s[i] > '9'))
			return false;
	}
	return true;
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
int32_t LSM6DSOX_DataGatherer::ptr_io_write(void *handle, uint8_t write_address, uint8_t *p_buffer,
											uint16_t number_bytes_to_write)
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
int32_t LSM6DSOX_DataGatherer::ptr_io_read(void *handle, uint8_t read_address, uint8_t *p_buffer,
										   uint16_t number_bytes_to_read)
{
	return (int32_t)((LSM6DSOX_DataGatherer *)handle)
		->_lsm6dsox_component_i2c.read(read_address, number_bytes_to_read, p_buffer);
}
}	// namespace Component