/**
******************************************************************************
* @file    CommunicationI2CBase.h
* @author  Yann Locatelli
* @brief   Abstract base class for I2C use by specific component.
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __COMMUNICATIONI2CBASE_H__
#define __COMMUNICATIONI2CBASE_H__

/* Includes ------------------------------------------------------------------*/
#include "CommunicationBase.h"
#include "mbed.h"
#include "pinmap.h"

namespace Communication {
	/* Defines -------------------------------------------------------------------*/
	/* Enums ---------------------------------------------------------------------*/
	/* Structs -------------------------------------------------------------------*/
	/* Unions --------------------------------------------------------------------*/
	/* Class Declaration ---------------------------------------------------------*/
	/**
	 * Abstract base class for I2C use by specific component.
	 */
	class I2CBase : CommunicationBase {
	  public:
		virtual ~I2CBase() {}
		virtual Status read(uint8_t register_address, uint16_t number_bytes_to_read,
							uint8_t *p_buffer);
		virtual Status write(uint8_t register_address, uint16_t number_bytes_to_write,
							 uint8_t *p_buffer);

		virtual void setAddress(uint8_t address) = 0;
		virtual uint8_t getAddress()			 = 0;
	};
}	// namespace Communication

#endif	 // __COMMUNICATIONI2CBASE_H__
