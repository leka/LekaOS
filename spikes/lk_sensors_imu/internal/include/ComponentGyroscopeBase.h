/**
******************************************************************************
* @file    ComponentGyroscopeBase.h
* @author  Yann Locatelli
* @brief   Abstract base class for gyroscope component.
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __COMPONENTGYROSCOPEBASE_H__
#define __COMPONENTGYROSCOPEBASE_H__

/* Includes ------------------------------------------------------------------*/
#include "ComponentBase.h"
#include "mbed.h"

namespace Component {
	/* Defines -------------------------------------------------------------------*/
	/* Enums ---------------------------------------------------------------------*/
	enum class GyroscopeRange : uint16_t {
		_125DPS	 = 125,
		_250DPS	 = 250,
		_500DPS	 = 500,
		_1000DPS = 1000,
		_2000DPS = 2000,
	};
	/* Structs -------------------------------------------------------------------*/
	/* Unions --------------------------------------------------------------------*/
	union GyroscopeRawData {
		std::array<int32_t, 3> raw_data;
		struct {
			int32_t x;
			int32_t y;
			int32_t z;
		} mdps;
	};

	union GyroscopeData {
		std::array<float, 3> data;
		struct {
			float x;
			float y;
			float z;
		} mdps;
	};

	union GyroscopeEvents {
		std::array<uint8_t, 16> component_events;
		struct {
			uint8_t data_ready;
		} event;
	};

	union GyroscopeEventsOnInterrupt {
		std::array<uint8_t, 16> component_events_on_interrupt;
		struct {
			uint8_t data_ready;
		} event_on_interrupt;
	};

	/* Class Declaration ---------------------------------------------------------*/
	/**
	 * Abstract base class for gyroscope component.
	 */
	class GyroscopeBase : ComponentBase {
	  public:
		virtual ~GyroscopeBase() {}

		virtual Status setDataRate(float data_rate)	 = 0;
		virtual Status getDataRate(float &data_rate) = 0;

		virtual Status setRange(GyroscopeRange range)  = 0;
		virtual Status getRange(GyroscopeRange &range) = 0;
	};
}	// namespace Component

#endif	 // __COMPONENTGYROSCOPEBASE_H__