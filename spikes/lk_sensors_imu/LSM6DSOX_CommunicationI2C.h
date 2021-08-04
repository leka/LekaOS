/**
******************************************************************************
* @file    LSM6DSOX_CommunicationI2C.h
* @author  Yann Locatelli
* @brief   Implementation class of LSM6DSOX_I2C based on CommunicationI2CBase.
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __LSM6DSOX_COMMUNICATIONI2C_H__
#define __LSM6DSOX_COMMUNICATIONI2C_H__

/* Includes ------------------------------------------------------------------*/
#include "CommunicationI2CBase.h"
#include "lsm6dsox_reg.h"

namespace Communication {
	/* Defines -------------------------------------------------------------------*/
	/* Enums ---------------------------------------------------------------------*/
	/* Structs -------------------------------------------------------------------*/
	/* Unions --------------------------------------------------------------------*/
	/* Class Declaration ---------------------------------------------------------*/
	/**
	 * Class for the use of I2C of LSM6DSOX components.
	 */
	class LSM6DSOX_I2C : public I2CBase {
	  public:
		LSM6DSOX_I2C(I2C &i2c, uint8_t address = LSM6DSOX_I2C_ADD_L);
		virtual ~LSM6DSOX_I2C() {}
		virtual Status read(uint8_t register_address, uint16_t number_bytes_to_read,
							uint8_t *p_buffer);
		virtual Status write(uint8_t register_address, uint16_t number_bytes_to_write,
							 uint8_t *p_buffer);
		virtual void setAddress(uint8_t address);
		virtual uint8_t getAddress();

	  private:
		I2C &_lsm6dsox_i2c;
		uint8_t _address;

		const uint16_t kBufferSize = 32;
	};
}	// namespace Communication

#endif	 // __LSM6DSOX_COMMUNICATIONI2C_H__