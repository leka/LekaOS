/**
******************************************************************************
* @file    LSM6DSOX_I2C.cpp
* @author  Yann Locatelli
* @brief   Implementation class of LSM6DSOX_I2C based on CommunicationI2CBase.
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "LSM6DSOX_CommunicationI2C.h"

namespace Communication {
	/* Class Implementation ------------------------------------------------------*/

	/** Constructor
	 * @param i2c object which handles the I2C peripheral
	 * @param address the address of the component
	 */
	LSM6DSOX_I2C::LSM6DSOX_I2C(I2C &i2c, uint8_t address) : _lsm6dsox_i2c(i2c), _address(address) {}

	/**
	 * @brief Utility function to read data.
	 * @param  register_address: specifies internal address register to be read.
	 * @param  number_byte_to_read: number of bytes to be read.
	 * @param  pBuffer: pointer to data to be read.
	 * @retval 0 if ok, an error code otherwise.
	 */
	Status LSM6DSOX_I2C::read(uint8_t register_address, uint16_t number_bytes_to_read,
							  uint8_t *pBuffer) {
		int ret;

		/* Send component address, with no STOP condition */
		ret = _lsm6dsox_i2c.write(_address, (const char *)&register_address, 1, true);
		if (!ret) {
			/* Read data, with STOP condition  */
			ret = _lsm6dsox_i2c.read(_address, (char *)pBuffer, number_bytes_to_read, false);
		}

		if (ret) return Status::ERROR;
		return Status::OK;
	}

	/**
	 * @brief Utility function to write data.
	 * @param  register_address: specifies internal address register to be written.
	 * @param  number_byte_to_write: number of bytes to be write.
	 * @param  pBuffer: pointer to data to be written.
	 * @retval 0 if ok, an error code otherwise.
	 */
	Status LSM6DSOX_I2C::write(uint8_t register_address, uint16_t number_bytes_to_write,
							   uint8_t *pBuffer) {

		int ret;
		uint8_t tmp[kBufferSize];

		if (number_bytes_to_write >= kBufferSize) return Status::overflow;

		/* First, send component address. Then, send data and STOP condition */
		tmp[0] = register_address;
		memcpy(tmp + 1, pBuffer, number_bytes_to_write);

		ret = _lsm6dsox_i2c.write(_address, (const char *)tmp, number_bytes_to_write + 1, false);

		if (ret) return Status::ERROR;
		return Status::OK;
	}

	/**
	 * @brief Set the address of component
	 * @param  address: i2c address of the component in hexadecimal value
	 */
	void LSM6DSOX_I2C::setAddress(uint8_t address) { _address = address; }

	/**
	 * @brief Get the address of component
	 * @retval i2c address of the component in hexadecimal value
	 */
	uint8_t LSM6DSOX_I2C::getAddress() { return _address; }

}	// namespace Communication