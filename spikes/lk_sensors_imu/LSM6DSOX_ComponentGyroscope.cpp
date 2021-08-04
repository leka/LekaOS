/**
******************************************************************************
* @file    LSM6DSOX_ComponentGyroscope.cpp
* @author  Yann Locatelli
* @brief   Implementation class of LSM6DSOX_ComponentGyroscope based on Component::GyroscopeBase.
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "LSM6DSOX_ComponentGyroscope.h"

namespace Component {
	/* Class Implementation ------------------------------------------------------*/

	/** Constructor
	 * @param Accelerometer component of the LSM6DSOX sensor
	 */
	LSM6DSOX_Gyroscope::LSM6DSOX_Gyroscope(Communication::I2CBase &component_i2c,
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
	Status LSM6DSOX_Gyroscope::init() {
		/* Initialize the component for driver usage */
		if (lsm6dsox_init_set(&_register_io_function, LSM6DSOX_DRV_RDY) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		setPowerMode(PowerMode::NORMAL);
		setDataRate(104.0f);
		setRange(GyroscopeRange::_125DPS);

		// TO DO : Disable OIS.

		return Status::OK;
	}

	/**
	 * @brief  Read component ID
	 * @param  id the WHO_AM_I value
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::getID(uint8_t &id) {
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
	Status LSM6DSOX_Gyroscope::setPowerMode(PowerMode power_mode) {
		if (power_mode == PowerMode::OFF) {
			if (lsm6dsox_gy_data_rate_set(&_register_io_function, LSM6DSOX_GY_ODR_OFF) !=
				(int32_t)Communication::Status::OK) {
				return Status::ERROR;
			}
			return Status::OK;
		}

		Status ret = Status::OK;
		lsm6dsox_g_hm_mode_t pm;
		switch (power_mode) {
			case PowerMode::NORMAL:
				pm = LSM6DSOX_GY_NORMAL;
				break;
			case PowerMode::HIGH_PERFORMANCE:
				pm = LSM6DSOX_GY_HIGH_PERFORMANCE;
				break;
			default:
				ret = Status::ERROR;
				break;
		}
		if (ret == Status::ERROR) { return Status::ERROR; }

		if (lsm6dsox_gy_power_mode_set(&_register_io_function, pm) !=
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
	Status LSM6DSOX_Gyroscope::getPowerMode(PowerMode &power_mode) {
		lsm6dsox_odr_g_t odr;

		if (lsm6dsox_gy_data_rate_get(&_register_io_function, &odr) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		if (odr != LSM6DSOX_GY_ODR_OFF) {
			power_mode = PowerMode::OFF;
			return Status::OK;
		}

		lsm6dsox_g_hm_mode_t pm;
		if (lsm6dsox_gy_power_mode_get(&_register_io_function, &pm) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		if (pm == LSM6DSOX_GY_NORMAL) {

			power_mode = PowerMode::NORMAL;
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
	Status LSM6DSOX_Gyroscope::setDataRate(float data_rate) {
		lsm6dsox_odr_g_t odr;
		if (data_rate <= 0) {
			odr = LSM6DSOX_GY_ODR_OFF;
		} else if (data_rate < 26.0f) {
			odr = LSM6DSOX_GY_ODR_12Hz5;
		} else if (data_rate < 52.0f) {
			odr = LSM6DSOX_GY_ODR_26Hz;
		} else if (data_rate < 104.0f) {
			odr = LSM6DSOX_GY_ODR_52Hz;
		} else if (data_rate < 208.0f) {
			odr = LSM6DSOX_GY_ODR_104Hz;
		} else if (data_rate < 417.0f) {
			odr = LSM6DSOX_GY_ODR_208Hz;
		} else if (data_rate < 833.0f) {
			odr = LSM6DSOX_GY_ODR_417Hz;
		} else if (data_rate < 1667.0f) {
			odr = LSM6DSOX_GY_ODR_833Hz;
		} else if (data_rate < 3333.0f) {
			odr = LSM6DSOX_GY_ODR_1667Hz;
		} else if (data_rate < 6667.0f) {
			odr = LSM6DSOX_GY_ODR_3333Hz;
		} else {
			odr = LSM6DSOX_GY_ODR_6667Hz;
		}

		if (lsm6dsox_gy_data_rate_set(&_register_io_function, odr) !=
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
	Status LSM6DSOX_Gyroscope::getDataRate(float &data_rate) {
		Status ret = Status::OK;
		lsm6dsox_odr_g_t odr;
		if (lsm6dsox_gy_data_rate_get(&_register_io_function, &odr) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		switch (odr) {
			case LSM6DSOX_GY_ODR_OFF:
				data_rate = 0;
				break;
			case LSM6DSOX_GY_ODR_12Hz5:
				data_rate = 12.5f;
				break;
			case LSM6DSOX_GY_ODR_26Hz:
				data_rate = 26.0f;
				break;
			case LSM6DSOX_GY_ODR_52Hz:
				data_rate = 52.0f;
				break;
			case LSM6DSOX_GY_ODR_104Hz:
				data_rate = 104.0f;
				break;
			case LSM6DSOX_GY_ODR_208Hz:
				data_rate = 208.0f;
				break;
			case LSM6DSOX_GY_ODR_417Hz:
				data_rate = 417.0f;
				break;
			case LSM6DSOX_GY_ODR_833Hz:
				data_rate = 833.0f;
				break;
			case LSM6DSOX_GY_ODR_1667Hz:
				data_rate = 1667.0f;
				break;
			case LSM6DSOX_GY_ODR_3333Hz:
				data_rate = 3333.0f;
				break;
			case LSM6DSOX_GY_ODR_6667Hz:
				data_rate = 6667.0f;
				break;
			default:
				ret = Status::ERROR;
				break;
		}
		return ret;
	}

	/**
	 * @brief  Set the range of the component
	 * @param  range is the range (in +/- dps) intended
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::setRange(GyroscopeRange range) {
		Status ret = Status::OK;
		lsm6dsox_fs_g_t fs;

		switch (range) {
			case GyroscopeRange::_125DPS:
				fs = LSM6DSOX_125dps;
				break;
			case GyroscopeRange::_250DPS:
				fs = LSM6DSOX_250dps;
				break;
			case GyroscopeRange::_500DPS:
				fs = LSM6DSOX_500dps;
				break;
			case GyroscopeRange::_1000DPS:
				fs = LSM6DSOX_1000dps;
				break;
			case GyroscopeRange::_2000DPS:
				fs = LSM6DSOX_2000dps;
				break;
			default:
				ret = Status::ERROR;
				break;
		}

		if (ret == Status::ERROR) { return Status::ERROR; }
		if (lsm6dsox_gy_full_scale_set(&_register_io_function, fs) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		return ret;
	}

	/**
	 * @brief  Get the range of the component
	 * @param  range is the range (in +/- dps) container
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::getRange(GyroscopeRange &range) {
		Status ret = Status::OK;
		lsm6dsox_fs_g_t fs;
		if (lsm6dsox_gy_full_scale_get(&_register_io_function, &fs) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		switch (fs) {
			case LSM6DSOX_125dps:
				range = GyroscopeRange::_125DPS;
				break;
			case LSM6DSOX_250dps:
				range = GyroscopeRange::_250DPS;
				break;
			case LSM6DSOX_500dps:
				range = GyroscopeRange::_500DPS;
				break;
			case LSM6DSOX_1000dps:
				range = GyroscopeRange::_1000DPS;
				break;
			case LSM6DSOX_2000dps:
				range = GyroscopeRange::_2000DPS;
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
	Status LSM6DSOX_Gyroscope::getRawData(std::array<int32_t, 3> &raw_data) {
		GyroscopeRawData gy = {{0}};
		uint8_t buff[6];
		if (lsm6dsox_angular_rate_raw_get(&_register_io_function, buff) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		// Every data is composed of 2 bytes, the first of them is the LSB.
		gy.mdps.x = (int32_t)((uint16_t)buff[1] << 8) | buff[0];
		gy.mdps.y = (int32_t)((uint16_t)buff[3] << 8) | buff[2];
		gy.mdps.z = (int32_t)((uint16_t)buff[5] << 8) | buff[4];

		raw_data = gy.raw_data;
		return Status::OK;
	}

	/**
	 * @brief  Get data
	 * @param  data is an array containing x, y and z axes in mdps
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::getData(std::array<float, 3> &data) {
		Status ret			  = Status::OK;
		GyroscopeData gy	  = {{0}};
		GyroscopeRawData gy_r = {{0}};
		GyroscopeRange fs;

		if (getRawData(gy_r.raw_data) != Status::OK) { return Status::ERROR; }
		if (getRange(fs) != Status::OK) { return Status::ERROR; }

		float_t (*pConversionFunction)(int16_t raw_value);
		switch (fs) {
			case GyroscopeRange::_125DPS:
				pConversionFunction = &lsm6dsox_from_fs125_to_mdps;
				break;
			case GyroscopeRange::_250DPS:
				pConversionFunction = &lsm6dsox_from_fs250_to_mdps;
				break;
			case GyroscopeRange::_500DPS:
				pConversionFunction = &lsm6dsox_from_fs500_to_mdps;
				break;
			case GyroscopeRange::_1000DPS:
				pConversionFunction = &lsm6dsox_from_fs1000_to_mdps;
				break;
			case GyroscopeRange::_2000DPS:
				pConversionFunction = &lsm6dsox_from_fs2000_to_mdps;
				break;
			default:
				ret = Status::ERROR;
				break;
		}

		if (ret == Status::ERROR) { return Status::ERROR; }

		gy.mdps.x = (float)(*pConversionFunction)(gy_r.mdps.x);
		gy.mdps.y = (float)(*pConversionFunction)(gy_r.mdps.y);
		gy.mdps.z = (float)(*pConversionFunction)(gy_r.mdps.z);

		data = gy.data;
		return Status::OK;
	}

	/**
	 * @brief  Get the pin of component interrupt on MCU
	 * @retval Pin of component interrupt on MCU
	 */
	PinName LSM6DSOX_Gyroscope::getPinInterrupt() { return _mcu_pin_interrupt; }

	/**
	 * @brief  Enable interrupts on the pin set up as _lsm6dsox_interrupt (Usually INT1)
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::enableInterrupt() {
		_lsm6dsox_interrupt.enable_irq();
		return Status::OK;
	}

	/**
	 * @brief  Disable interrupts on the pin set up as _lsm6dsox_interrupt (Usually INT1)
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::disableInterrupt() {
		_lsm6dsox_interrupt.disable_irq();
		return Status::OK;
	}

	/**
	 * @brief  Read interrupt from component
	 * @param  interrupt_status contain the value on pin interrupt of component
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::readInterrupt(uint8_t &interrupt_status) {
		interrupt_status = _lsm6dsox_interrupt;
		return Status::OK;
	}

	/**
	 * @brief  Attaching an interrupt handler to the component interrupt
	 * @param  func An interrupt handler.
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::attachInterrupt(Callback<void()> func) {
		_lsm6dsox_interrupt.rise(func);
		return Status::OK;
	}

	/**
	 * @brief  Get status of events on component
	 * @param  component_events array of events available. Check GyroscopeComponentEvents for more
	 * details
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::getEventStatus(std::array<uint8_t, 16> &component_events) {
		GyroscopeEvents gy;
		lsm6dsox_status_t events;

		if (lsm6dsox_status_get(&_register_io_function, NULL, &events) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		gy.event.data_ready = events.drdy_g;

		component_events = gy.component_events;
		return Status::OK;
	}

	/**
	 * @brief  Set events on interrupt of component
	 * @param  component_events_on_interrupt array of events on interrupt available. Check
	 * GyroscopeComponentEventsOnInterrupt for more details
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::setEventsOnInterrupt(
		std::array<uint8_t, 16> &component_events_on_interrupt) {
		GyroscopeEventsOnInterrupt gy = {{component_events_on_interrupt}};

		lsm6dsox_pin_int1_route_t interrupts_1;
		if (lsm6dsox_pin_int1_route_get(&_register_io_function, &interrupts_1) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		interrupts_1.drdy_g = gy.event_on_interrupt.data_ready;
		if (lsm6dsox_pin_int1_route_set(&_register_io_function, interrupts_1) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		lsm6dsox_pin_int2_route_t interrupts_2;
		if (lsm6dsox_pin_int2_route_get(&_register_io_function, NULL, &interrupts_2) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}
		interrupts_2.drdy_g = gy.event_on_interrupt.data_ready;
		if (lsm6dsox_pin_int2_route_set(&_register_io_function, NULL, interrupts_2) !=
			(int32_t)Communication::Status::OK) {
			return Status::ERROR;
		}

		return Status::OK;
	}

	/**
	 * @brief  Get events on interrupt of component
	 * @param  component_events_on_interrupt array of events on interrupt available. Check
	 * GyroscopeComponentEventsOnInterrupt for more details
	 * @retval 0 in case of success, an error code otherwise
	 */
	Status LSM6DSOX_Gyroscope::getEventsOnInterrupt(
		std::array<uint8_t, 16> &component_events_on_interrupt) {
		GyroscopeEventsOnInterrupt gy;

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

		gy.event_on_interrupt.data_ready = (interrupts_1.drdy_g | interrupts_2.drdy_g);

		component_events_on_interrupt = gy.component_events_on_interrupt;
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
	int32_t LSM6DSOX_Gyroscope::ptr_io_write(void *handle, uint8_t write_address, uint8_t *p_buffer,
											 uint16_t number_bytes_to_write) {
		return (int32_t)((LSM6DSOX_Gyroscope *)handle)
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
	int32_t LSM6DSOX_Gyroscope::ptr_io_read(void *handle, uint8_t read_address, uint8_t *p_buffer,
											uint16_t number_bytes_to_read) {
		return (int32_t)((LSM6DSOX_Gyroscope *)handle)
			->_lsm6dsox_component_i2c.read(read_address, number_bytes_to_read, p_buffer);
	}

}	// namespace Component