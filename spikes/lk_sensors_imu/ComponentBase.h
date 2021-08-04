/**
******************************************************************************
* @file    ComponentBase.h
* @author  Yann Locatelli
* @brief   Abstract base class for any component (sensor, actuator, ...)
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __COMPONENTBASE_H__
#define __COMPONENTBASE_H__

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"

namespace Component {
	/* Defines -------------------------------------------------------------------*/
	/* Enums ---------------------------------------------------------------------*/
	enum class Status : uint8_t {
		OK			  = 0,
		ERROR		  = 1,
		NOT_AVAILABLE = 2,
	};

	enum class PowerMode {
		HIGH_PERFORMANCE,
		NORMAL,
		LOW,
		ULTRA_LOW,
		OFF,
	};

	/* Structs -------------------------------------------------------------------*/
	/* Unions --------------------------------------------------------------------*/
	/* Class Declaration ---------------------------------------------------------*/
	/**
	 * Abstract base class for any component.
	 */
	class ComponentBase {
	  public:
		virtual ~ComponentBase() {}
		virtual Status init()			  = 0;
		virtual Status getID(uint8_t &id) = 0;

		virtual Status setPowerMode(PowerMode power_mode)  = 0;
		virtual Status getPowerMode(PowerMode &power_mode) = 0;

		virtual Status getRawData(std::array<int32_t, 3> &raw_data) = 0;
		virtual Status getData(std::array<float, 3> &data)			= 0;

		// virtual  Status setPinInterrupt(PinName pin_interrupt)	  = 0;
		virtual PinName getPinInterrupt()						= 0;
		virtual Status enableInterrupt()						= 0;
		virtual Status disableInterrupt()						= 0;
		virtual Status readInterrupt(uint8_t &interrupt_status) = 0;
		virtual Status attachInterrupt(Callback<void()> func)	= 0;

		// Some component can generate more than one event, component_events list them.
		virtual Status getEventStatus(std::array<uint8_t, 16> &component_events) = 0;

		// Some component allows events to trigger the interrupt, component_events_on_interrupt list
		// them.
		virtual Status setEventsOnInterrupt(
			std::array<uint8_t, 16> &component_events_on_interrupt) = 0;
		virtual Status getEventsOnInterrupt(
			std::array<uint8_t, 16> &component_events_on_interrupt) = 0;

		// TO DO add offset
	};
}	// namespace Component

#endif	 // __COMPONENTBASE_H__