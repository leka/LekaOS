/**
******************************************************************************
* @file    LSM6DSOX_ComponentAccelerometer.cpp
* @author  Yann Locatelli
* @brief   Implementation class of LSM6DSOX_Accelerometer based on Component::AccelerometerBase.
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "LSM6DSOX_ComponentAccelerometer.h"

namespace Component {
	/* Class Implementation ------------------------------------------------------*/

	/** Constructor
	 * @param Accelerometer component of the LSM6DSOX sensor
	 */
	LSM6DSOX_Accelerometer::LSM6DSOX_Accelerometer(Communication::I2CBase &component_i2c,
												   PinName pin_interrupt)
		: _lsm6dsox_component_i2c(component_i2c),
		  _mcu_pin_interrupt(pin_interrupt),
		  _lsm6dsox_interrupt(pin_interrupt) {
		_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
		_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
		_register_io_function.handle	= (void *)this;
	}

	/**
	 * @brief  Initializing the component
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::init() {
		/* Initialize the component for driver usage */
		if (lsm6dsox_init_set(&_register_io_function, LSM6DSOX_DRV_RDY) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		setPowerMode(PowerMode::NORMAL);
		setDataRate(104.0f);
		setRange(AccelerometerRange::_2G);

		// TO DO : Disable OIS.
		
		return Status::OK;
	}

	/**
	 * @brief  Read component ID
	 * @param  id the WHO_AM_I value
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::getID(uint8_t &id) {
		uint8_t p_data = 0;
		if (lsm6dsox_device_id_get(&_register_io_function, &p_data) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		id = p_data;

		return Status::OK;
	}

	/**
	 * @brief  Set the power mode of the component
	 * @param  power_mode is the power mode intended
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::setPowerMode(PowerMode power_mode) {
		if (power_mode == PowerMode::OFF) {
			if (lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF) !=
				(int32_t)Communication::Status::OK) {
				return Status::ERROR;
			}
			return Status::OK;
		}

		Status ret = Status::OK;
		lsm6dsox_xl_hm_mode_t pm;
		switch (power_mode) {
			case PowerMode::ULTRA_LOW:
				pm = LSM6DSOX_ULTRA_LOW_POWER_MD;
				break;
			case PowerMode::LOW:
				pm = LSM6DSOX_LOW_NORMAL_POWER_MD;
				break;
			case PowerMode::NORMAL:
				pm = LSM6DSOX_LOW_NORMAL_POWER_MD;
				break;
			case PowerMode::HIGH_PERFORMANCE:
				pm = LSM6DSOX_HIGH_PERFORMANCE_MD;
				break;
			default:
				ret = Status::ERROR;
				break;
		}
		if (ret == Status::ERROR) { return Status::ERROR; }

		if (lsm6dsox_xl_power_mode_set(&_register_io_function, pm) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		return ret;
	}

	/**
	 * @brief  Get the power mode of the component
	 * @param  power_mode is the power mode container
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::getPowerMode(PowerMode &power_mode) {
		lsm6dsox_odr_xl_t odr;

		if (lsm6dsox_xl_data_rate_get(&_register_io_function, &odr) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		if (odr != LSM6DSOX_XL_ODR_OFF) {
			power_mode = PowerMode::OFF;
			return Status::OK;
		}

		lsm6dsox_xl_hm_mode_t pm;
		if (lsm6dsox_xl_power_mode_get(&_register_io_function, &pm) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		if (pm == LSM6DSOX_ULTRA_LOW_POWER_MD) {
			power_mode = PowerMode::ULTRA_LOW;
		} else if (pm == LSM6DSOX_LOW_NORMAL_POWER_MD) {
			if (odr == LSM6DSOX_XL_ODR_1Hz6 || odr == LSM6DSOX_XL_ODR_12Hz5 ||
				odr == LSM6DSOX_XL_ODR_26Hz || odr == LSM6DSOX_XL_ODR_52Hz) {
				power_mode = PowerMode::LOW;
			} else {
				power_mode = PowerMode::NORMAL;
			}
		} else {
			power_mode = PowerMode::HIGH_PERFORMANCE;
		}

		return Status::OK;
	}

	/**
	 * @brief  Set the data rate of the component
	 * @param  data_rate is the data rate (in Hz) intended
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::setDataRate(float data_rate) {
		lsm6dsox_odr_xl_t odr;
		if (data_rate <= 0) {
			odr = LSM6DSOX_XL_ODR_OFF;
		} else if (data_rate < 12.5f) {
			odr = LSM6DSOX_XL_ODR_1Hz6;
		} else if (data_rate < 26.0f) {
			odr = LSM6DSOX_XL_ODR_12Hz5;
		} else if (data_rate < 52.0f) {
			odr = LSM6DSOX_XL_ODR_26Hz;
		} else if (data_rate < 104.0f) {
			odr = LSM6DSOX_XL_ODR_52Hz;
		} else if (data_rate < 208.0f) {
			odr = LSM6DSOX_XL_ODR_104Hz;
		} else if (data_rate < 417.0f) {
			odr = LSM6DSOX_XL_ODR_208Hz;
		} else if (data_rate < 833.0f) {
			odr = LSM6DSOX_XL_ODR_417Hz;
		} else if (data_rate < 1667.0f) {
			odr = LSM6DSOX_XL_ODR_833Hz;
		} else if (data_rate < 3333.0f) {
			odr = LSM6DSOX_XL_ODR_1667Hz;
		} else if (data_rate < 6667.0f) {
			odr = LSM6DSOX_XL_ODR_3333Hz;
		} else {
			odr = LSM6DSOX_XL_ODR_6667Hz;
		}

		if (lsm6dsox_xl_data_rate_set(&_register_io_function, odr) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		return Status::OK;
	}

	/**
	 * @brief  Get the data rate of the component
	 * @param  data_rate is the data rate (in Hz) container
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::getDataRate(float &data_rate) {
		Status ret = Status::OK;
		lsm6dsox_odr_xl_t odr;
		if (lsm6dsox_xl_data_rate_get(&_register_io_function, &odr) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		switch (odr) {
			case LSM6DSOX_XL_ODR_OFF:
				data_rate = 0;
				break;
			case LSM6DSOX_XL_ODR_12Hz5:
				data_rate = 12.5f;
				break;
			case LSM6DSOX_XL_ODR_26Hz:
				data_rate = 26.0f;
				break;
			case LSM6DSOX_XL_ODR_52Hz:
				data_rate = 52.0f;
				break;
			case LSM6DSOX_XL_ODR_104Hz:
				data_rate = 104.0f;
				break;
			case LSM6DSOX_XL_ODR_208Hz:
				data_rate = 208.0f;
				break;
			case LSM6DSOX_XL_ODR_417Hz:
				data_rate = 417.0f;
				break;
			case LSM6DSOX_XL_ODR_833Hz:
				data_rate = 833.0f;
				break;
			case LSM6DSOX_XL_ODR_1667Hz:
				data_rate = 1667.0f;
				break;
			case LSM6DSOX_XL_ODR_3333Hz:
				data_rate = 3333.0f;
				break;
			case LSM6DSOX_XL_ODR_6667Hz:
				data_rate = 6667.0f;
				break;
			case LSM6DSOX_XL_ODR_1Hz6:
				data_rate = 1.6f;
				break;
			default:
				ret = Status::ERROR;
				break;
		}
		return ret;
	}

	/**
	 * @brief  Set the range of the component
	 * @param  range is the range (in +/- g) intended
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::setRange(AccelerometerRange range) {
		Status ret = Status::OK;
		lsm6dsox_fs_xl_t fs;

		switch (range) {
			case AccelerometerRange::_2G:
				fs = LSM6DSOX_2g;
				break;
			case AccelerometerRange::_4G:
				fs = LSM6DSOX_4g;
				break;
			case AccelerometerRange::_8G:
				fs = LSM6DSOX_8g;
				break;
			case AccelerometerRange::_16G:
				fs = LSM6DSOX_16g;
				break;
			default:
				ret = Status::ERROR;
				break;
		}

		if (ret == Status::ERROR) { return Status::ERROR; }
		if (lsm6dsox_xl_full_scale_set(&_register_io_function, fs) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		return ret;
	}

	/**
	 * @brief  Get the range of the component
	 * @param  range is the range (in +/- g) container
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::getRange(AccelerometerRange &range) {
		Status ret = Status::OK;
		lsm6dsox_fs_xl_t fs;
		if (lsm6dsox_xl_full_scale_get(&_register_io_function, &fs) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		switch (fs) {
			case LSM6DSOX_2g:
				range = AccelerometerRange::_2G;
				break;
			case LSM6DSOX_4g:
				range = AccelerometerRange::_4G;
				break;
			case LSM6DSOX_8g:
				range = AccelerometerRange::_8G;
				break;
			case LSM6DSOX_16g:
				range = AccelerometerRange::_16G;
				break;
			default:
				ret = Status::ERROR;
				break;
		}
		return ret;
	}

	/**
	 * @brief  Get raw data
	 * @param  raw_data is an array containing x, y and z axes in bytes
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::getRawData(std::array<int32_t, 3> &raw_data) {
		AccelerometerRawData xl = {{0}};
		uint8_t buff[6];
		if (lsm6dsox_acceleration_raw_get(&_register_io_function, buff) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		// Every data is composed of 2 bytes, the first of them is the LSB.
		xl.mg.x = (int32_t)((uint16_t)buff[1] << 8) | buff[0];
		xl.mg.y = (int32_t)((uint16_t)buff[3] << 8) | buff[2];
		xl.mg.z = (int32_t)((uint16_t)buff[5] << 8) | buff[4];

		raw_data = xl.raw_data;
		return Status::OK;
	}

	/**
	 * @brief  Get data
	 * @param  data is an array containing x, y and z axes in mg
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::getData(std::array<float, 3> &data) {
		Status ret				  = Status::OK;
		AccelerometerData xl	  = {{0}};
		AccelerometerRawData xl_r = {{0}};
		AccelerometerRange fs;

		if (getRawData(xl_r.raw_data) != Status::OK) { return Status::ERROR; }
		if (getRange(fs) != Status::OK) { return Status::ERROR; }

		float_t (*pConversionFunction)(int16_t raw_value);
		switch (fs) {
			case AccelerometerRange::_2G:
				pConversionFunction = &lsm6dsox_from_fs2_to_mg;
				break;
			case AccelerometerRange::_4G:
				pConversionFunction = &lsm6dsox_from_fs4_to_mg;
				break;
			case AccelerometerRange::_8G:
				pConversionFunction = &lsm6dsox_from_fs8_to_mg;
				break;
			case AccelerometerRange::_16G:
				pConversionFunction = &lsm6dsox_from_fs16_to_mg;
				break;
			default:
				ret = Status::ERROR;
				break;
		}

		if (ret == Status::ERROR) { return Status::ERROR; }

		xl.mg.x = (float)(*pConversionFunction)(xl_r.mg.x);
		xl.mg.y = (float)(*pConversionFunction)(xl_r.mg.y);
		xl.mg.z = (float)(*pConversionFunction)(xl_r.mg.z);

		data = xl.data;
		return Status::OK;
	}

	/**
	 * @brief  Get the pin of component interrupt on MCU
	 * @retval Pin of component interrupt on MCU
	 */
	PinName LSM6DSOX_Accelerometer::getPinInterrupt() { return _mcu_pin_interrupt; }

	/**
	 * @brief  Enable interrupts on the pin set up as _lsm6dsox_interrupt (Usually INT1)
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::enableInterrupt() {
		_lsm6dsox_interrupt.enable_irq();
		return Status::OK;
	}

	/**
	 * @brief  Disable interrupts on the pin set up as _lsm6dsox_interrupt (Usually INT1)
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::disableInterrupt() {
		_lsm6dsox_interrupt.disable_irq();
		return Status::OK;
	}

	/**
	 * @brief  Read interrupt from component
	 * @param  interrupt_status contain the value on pin interrupt of component
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::readInterrupt(uint8_t &interrupt_status) {
		interrupt_status = _lsm6dsox_interrupt;
		return Status::OK;
	}

	/**
	 * @brief  Attaching an interrupt handler to the component interrupt
	 * @param  func An interrupt handler.
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::attachInterrupt(Callback<void()> func) {
		_lsm6dsox_interrupt.rise(func);
		return Status::OK;
	}

	/**
	 * @brief  Get status of events on component
	 * @param  component_events array of events available. Check AccelerometerComponentEvents for
	 * more details
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::getEventStatus(std::array<uint8_t, 16> &component_events) {
		AccelerometerEvents xl;
		lsm6dsox_status_t events;

		if (lsm6dsox_status_get(&_register_io_function, NULL, &events) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		xl.event.data_ready = events.drdy_xl;

		component_events = xl.component_events;
		return Status::OK;
	}

	/**
	 * @brief  Set events on interrupt of component
	 * @param  component_events_on_interrupt array of events on interrupt available. Check
	 * AccelerometerComponentEventsOnInterrupt for more details
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::setEventsOnInterrupt(
		std::array<uint8_t, 16> &component_events_on_interrupt) {
		AccelerometerEventsOnInterrupt xl = {{component_events_on_interrupt}};

		lsm6dsox_pin_int1_route_t interrupts_1;
		if (lsm6dsox_pin_int1_route_get(&_register_io_function, &interrupts_1) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		interrupts_1.drdy_xl = xl.event_on_interrupt.data_ready;
		if (lsm6dsox_pin_int1_route_set(&_register_io_function, interrupts_1) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		lsm6dsox_pin_int2_route_t interrupts_2;
		if (lsm6dsox_pin_int2_route_get(&_register_io_function, NULL, &interrupts_2) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		interrupts_2.drdy_xl = xl.event_on_interrupt.data_ready;
		if (lsm6dsox_pin_int2_route_set(&_register_io_function, NULL, interrupts_2) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		return Status::OK;
	}

	/**
	 * @brief  Get events on interrupt of component
	 * @param  component_events_on_interrupt array of events on interrupt available. Check
	 * AccelerometerComponentEventsOnInterrupt for more details
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Accelerometer::getEventsOnInterrupt(
		std::array<uint8_t, 16> &component_events_on_interrupt) {
		AccelerometerEventsOnInterrupt xl;

		lsm6dsox_pin_int1_route_t interrupts_1;
		if (lsm6dsox_pin_int1_route_get(&_register_io_function, &interrupts_1) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		lsm6dsox_pin_int2_route_t interrupts_2;
		if (lsm6dsox_pin_int2_route_get(&_register_io_function, NULL, &interrupts_2) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		xl.event_on_interrupt.data_ready = (interrupts_1.drdy_xl | interrupts_2.drdy_xl);

		component_events_on_interrupt = xl.component_events_on_interrupt;
		return Status::OK;
	}

	/**
	 * @brief  Function used by driver to write data.
	 * @param  handle current object
	 * @param  write_address address of the register to write on
	 * @param  p_buffer contains data to write
	 * @param  number_bytes_to_write number of data to write in bytes
	 * @retval 0 in case of success, an error code otherwise
	 */
	int32_t LSM6DSOX_Accelerometer::ptr_io_write(void *handle, uint8_t write_address,
												 uint8_t *p_buffer,
												 uint16_t number_bytes_to_write) {
		return (int32_t)((LSM6DSOX_Accelerometer *)handle)
			->_lsm6dsox_component_i2c.write(write_address, number_bytes_to_write, p_buffer);
	}

	/**
	 * @brief  Function used by driver to read data.
	 * @param  handle current object
	 * @param  read_address address of the register to read on
	 * @param  p_buffer contains data to read
	 * @param  number_bytes_to_write number of data to read in bytes
	 * @retval 0 in case of success, an error code otherwise
	 */
	int32_t LSM6DSOX_Accelerometer::ptr_io_read(void *handle, uint8_t read_address,
												uint8_t *p_buffer, uint16_t number_bytes_to_read) {
		return (int32_t)((LSM6DSOX_Accelerometer *)handle)
			->_lsm6dsox_component_i2c.read(read_address, number_bytes_to_read, p_buffer);
	}
}	// namespace Component
