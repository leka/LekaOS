/**
******************************************************************************
* @file    CommunicationBase.h
* @author  Yann Locatelli
* @brief   Abstract base class for communication use by specific component.
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __COMMUNICATIONBASE_H__
#define __COMMUNICATIONBASE_H__

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"

#include "pinmap.h"

namespace Communication {
/* Defines -------------------------------------------------------------------*/
/* Enums ---------------------------------------------------------------------*/
enum class Status : uint8_t
{
	OK		 = 0,
	ERROR	 = 1,
	overflow = 2,
};
/* Structs -------------------------------------------------------------------*/
/* Unions --------------------------------------------------------------------*/
/* Class Declaration ---------------------------------------------------------*/
/**
 * Abstract base class for communication use by specific component.
 */
class CommunicationBase
{
  public:
	virtual ~CommunicationBase() = default;
};
}	// namespace Communication

#endif	 // __COMMUNICATIONBASE_H__
