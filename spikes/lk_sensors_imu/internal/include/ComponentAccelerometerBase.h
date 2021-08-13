/**
******************************************************************************
* @file    ComponentAccelerometerBase.h
* @author  Yann Locatelli
* @brief   Abstract base class for accelerometer component.
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __COMPONENTACCELEROMETERBASE_H__
#define __COMPONENTACCELEROMETERBASE_H__

/* Includes ------------------------------------------------------------------*/
#include "ComponentBase.h"
#include "mbed.h"

namespace Component {
	/* Defines -------------------------------------------------------------------*/
	/* Enums ---------------------------------------------------------------------*/
	enum class AccelerometerRange : uint8_t {
		_2G	 = 2,
		_4G	 = 4,
		_8G	 = 8,
		_16G = 16,
	};
	/* Structs -------------------------------------------------------------------*/
	/* Unions --------------------------------------------------------------------*/
	union AccelerometerRawData {
		std::array<int32_t, 3> raw_data;
		struct {
			int32_t x;
			int32_t y;
			int32_t z;
		} mg;
	};

	union AccelerometerData {
		std::array<float, 3> data;
		struct {
			float x;
			float y;
			float z;
		} mg;
	};

	union AccelerometerEvents {
		std::array<uint8_t, 16> component_events;
		struct {
			uint8_t data_ready;
		} event;
	};

	union AccelerometerEventsOnInterrupt {
		std::array<uint8_t, 16> component_events_on_interrupt;
		struct {
			uint8_t data_ready;
		} event_on_interrupt;
	};

	/* Class Declaration ---------------------------------------------------------*/
	/**
	 * Abstract base class for accelerometer component.
	 */
	class AccelerometerBase : ComponentBase {
	  public:
		virtual ~AccelerometerBase() {}

		virtual Status setDataRate(float data_rate)	 = 0;
		virtual Status getDataRate(float &data_rate) = 0;

		virtual Status setRange(AccelerometerRange range)  = 0;
		virtual Status getRange(AccelerometerRange &range) = 0;
	};
}	// namespace Component

#endif	 // __COMPONENTACCELEROMETERBASE_H__