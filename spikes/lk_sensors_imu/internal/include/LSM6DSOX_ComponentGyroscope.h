/**
******************************************************************************
* @file    LSM6DSOX_ComponentGyroscope.h
* @author  Yann Locatelli
* @brief   Implementation of Component::GyroscopeBase for LSM6DSOX Gyroscope
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __LSM6DSOX_COMPONENTGYROSCOPE_H__
#define __LSM6DSOX_COMPONENTGYROSCOPE_H__

/* Includes ------------------------------------------------------------------*/
#include "CommunicationI2CBase.h"
#include "ComponentGyroscopeBase.h"
#include "lsm6dsox_reg.h"

namespace Component {
	/* Defines -------------------------------------------------------------------*/
	/* Enums ---------------------------------------------------------------------*/
	/* Structs -------------------------------------------------------------------*/
	/* Unions --------------------------------------------------------------------*/
	/* Class Declaration ---------------------------------------------------------*/
	/**
	 * Class for the use of LSM6DSOX component.
	 */
	class LSM6DSOX_Gyroscope : GyroscopeBase {
	  public:
		LSM6DSOX_Gyroscope(Communication::I2CBase &component_i2c, PinName pin_interrupt);
		virtual Status init();
		virtual Status getID(uint8_t &id);

		virtual Status setPowerMode(PowerMode power_mode);
		virtual Status getPowerMode(PowerMode &power_mode);

		virtual Status setDataRate(float data_rate);
		virtual Status getDataRate(float &data_rate);

		virtual Status setRange(GyroscopeRange range);
		virtual Status getRange(GyroscopeRange &range);

		virtual Status getRawData(std::array<int32_t, 3> &raw_data);
		virtual Status getData(std::array<float, 3> &data);

		virtual PinName getPinInterrupt();
		virtual Status enableInterrupt();
		virtual Status disableInterrupt();
		virtual Status readInterrupt(uint8_t &interrupt_status);
		virtual Status attachInterrupt(Callback<void()> func);

		// Some component can generate more than one event, component_events list them.
		virtual Status getEventStatus(std::array<uint8_t, 16> &component_events);

		// Some component allows events to trigger the interrupt, component_events_on_interrupt list
		// them.
		virtual Status setEventsOnInterrupt(std::array<uint8_t, 16> &component_events_on_interrupt);
		virtual Status getEventsOnInterrupt(std::array<uint8_t, 16> &component_events_on_interrupt);

	  protected:
		static int32_t ptr_io_write(void *handle, uint8_t write_address, uint8_t *p_buffer,
									uint16_t number_bytes_to_write);
		static int32_t ptr_io_read(void *handle, uint8_t read_address, uint8_t *p_buffer,
								   uint16_t number_bytes_to_read);

	  private:
		Communication::I2CBase &_lsm6dsox_component_i2c;
		stmdev_ctx_t _register_io_function;
		PinName _mcu_pin_interrupt;
		InterruptIn _lsm6dsox_interrupt;
	};
}	// namespace Component

#endif	 // __LSM6DSOX_COMPONENTGYROSCOPE_H__