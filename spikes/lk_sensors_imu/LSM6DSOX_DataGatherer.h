/**
******************************************************************************
* @file    LSM6DSOX_DataGatherer.h
* @author  Maxime Blanc and Samuel Hadjes
* @brief   Data Gathering class for the LSM6DSOX sensor
******************************************************************************
*/

/* Prevent recursive inclusion -----------------------------------------------*/
#ifndef __LSM6DSOX_DATAGATHERER_H__
#define __LSM6DSOX_DATAGATHERER_H__

/* Includes ------------------------------------------------------------------*/
#include "CommunicationI2CBase.h"
#include "LSM6DSOX_ComponentAccelerometer.h"
#include "LSM6DSOX_ComponentGyroscope.h"
#include "lsm6dsox_reg.h"

namespace Component {
/* Defines -------------------------------------------------------------------*/
/* Enums ---------------------------------------------------------------------*/
enum class SerialCommand : uint8_t
{
	one,
	two,
	three
};
/* Structs -------------------------------------------------------------------*/
/* Unions --------------------------------------------------------------------*/
union Lsm6dsoxDataToSend {
	std::array<bool, 6> dataArray;
	struct {
		bool acc_x;
		bool acc_y;
		bool acc_z;
		bool gyr_x;
		bool gyr_y;
		bool gyr_z;
	} data;
};

union DataGathererData {
	std::array<float, 6> array;
	struct {
		AccelerometerData acc;
		GyroscopeData gy;
	} values;
};
/* Class Declaration ---------------------------------------------------------*/
/**
 * @brief Machine Leraning Core class, inherit from MachineLearningCoreBase
 *
 */
class LSM6DSOX_DataGatherer
{
  private:
	Communication::I2CBase &_lsm6dsox_component_i2c;

	PinName _mcu_pin_interrupt1;
	InterruptIn _lsm6dsox_interrupt1;

	Component::LSM6DSOX_Accelerometer _accelerometer;
	Component::LSM6DSOX_Gyroscope _gyroscope;

	Ticker _ticker;

	bool _serialStrComplete;

	float _dataRate;

	stmdev_ctx_t _register_io_function;
	EventQueue *_queue;

	string _serialInputStr;
	UnbufferedSerial _serial;
	void (*_tickerCB)();

	// Event<void()> _serialHandler;

  public:
	LSM6DSOX_DataGatherer(Communication::I2CBase &component_i2c, PinName pin_interrupt1, float dataRate, PinName tx,
						  PinName rx, int baud, void (*callBack)());

	virtual void printHeader();
	virtual void printData(DataGathererData data);

	virtual void onSerialReceived();
	virtual void parseCommand();

	virtual auto isFloat(string s) -> bool;

	virtual void sendIntBinary(int var);

	virtual void onTick();

	virtual auto init() -> Component::Status;
	virtual auto disableI3C() -> Component::Status;
	virtual auto restoreDefaultConfiguration() -> Component::Status;

	virtual auto getID(uint8_t &id) -> Component::Status;

	// temporary, to simplify developpement
	// TODO erase this once no more in use
	virtual auto TMP_getIoFunc() -> stmdev_ctx_t *;

	virtual void setDataRate(float data_rate);
	virtual void setDataRateWhileRunning(float data_rate);

	virtual void getDataRate(float &data_rate);

	virtual auto getData(std::array<float, 6> &data) -> Component::Status;

	virtual auto getInterruptPin() -> PinName;
	virtual auto enableInterrupt() -> Component::Status;
	virtual auto disableInterrupt() -> Component::Status;

	// virtual Component::Status readInterrupt(uint8_t &interrupt_status);
	// virtual Component::Status attachInterrupt(Callback<void()> func);

	virtual void startTicker();
	virtual void stopTicker();

  protected:
	static auto ptr_io_write(void *handle, uint8_t write_address, uint8_t *p_buffer, uint16_t number_bytes_to_write)
		-> int32_t;
	static auto ptr_io_read(void *handle, uint8_t read_address, uint8_t *p_buffer, uint16_t number_bytes_to_read)
		-> int32_t;
};
}	// namespace Component
#endif	 // __LSM6DSOX_DATAGATHERER_H__