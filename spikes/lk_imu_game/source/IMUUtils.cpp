#include "include/IMUUtils.h"
#include <cstdint>

namespace leka {

LKCoreIMU::LKCoreIMU(mbed::I2C &interface, PinName pin_interrupt)
	: _interface(interface), _lsm6dsox_interrupt(pin_interrupt)
{
	_register_io_function.write_reg = (stmdev_write_ptr)ptr_io_write;
	_register_io_function.read_reg	= (stmdev_read_ptr)ptr_io_read;
	_register_io_function.handle	= (void *)this;

	mbed::DigitalOut INT_1_LSM6DSOX(_lsm6dsox_interrupt, 0);

	lsm6dsox_init_set(&_register_io_function, LSM6DSOX_DRV_RDY);
	lsm6dsox_i3c_disable_set(&_register_io_function, LSM6DSOX_I3C_DISABLE);

	lsm6dsox_mode_get(&_register_io_function, NULL, &_config);

	_config.ui.xl.odr = _config.ui.xl.LSM6DSOX_XL_UI_104Hz_NM;
	_config.ui.xl.fs  = _config.ui.xl.LSM6DSOX_XL_UI_2g;
	_config.ui.gy.odr = _config.ui.gy.LSM6DSOX_GY_UI_104Hz_NM;
	_config.ui.gy.fs  = _config.ui.gy.LSM6DSOX_GY_UI_125dps;

	lsm6dsox_mode_set(&_register_io_function, NULL, &_config);
}

void LKCoreIMU::getData(std::array<float, 3> &xl_data, std::array<float, 3> &gy_data)
{
	lsm6dsox_data_t data;

	lsm6dsox_data_get(&_register_io_function, NULL, &_config, &data);

	xl_data[0] = data.ui.xl.mg[0];
	xl_data[1] = data.ui.xl.mg[1];
	xl_data[2] = data.ui.xl.mg[2];
	gy_data[0] = data.ui.gy.mdps[0];
	gy_data[1] = data.ui.gy.mdps[1];
	gy_data[2] = data.ui.gy.mdps[2];
}

int32_t LKCoreIMU::ptr_io_write(void *handle, uint8_t write_address, uint8_t *p_buffer, uint16_t number_bytes_to_write)
{
	return (int32_t)((LKCoreIMU *)handle)->write(write_address, number_bytes_to_write, p_buffer);
}

int32_t LKCoreIMU::ptr_io_read(void *handle, uint8_t read_address, uint8_t *p_buffer, uint16_t number_bytes_to_read)
{
	return (int32_t)((LKCoreIMU *)handle)->read(read_address, number_bytes_to_read, p_buffer);
}

int32_t LKCoreIMU::read(uint8_t register_address, uint16_t number_bytes_to_read, uint8_t *pBuffer)
{
	int ret;

	/* Send component address, with no STOP condition */
	ret = _interface.write(_address, (const char *)&register_address, 1, true);
	if (!ret) {
		/* Read data, with STOP condition  */
		ret = _interface.read(_address, (char *)pBuffer, number_bytes_to_read, false);
	}

	if (ret) return 1;
	return 0;
}

int32_t LKCoreIMU::write(uint8_t register_address, uint16_t number_bytes_to_write, uint8_t *pBuffer)
{
	int ret;
	uint8_t tmp[32];

	if (number_bytes_to_write >= 32) return 2;

	/* First, send component address. Then, send data and STOP condition */
	tmp[0] = register_address;
	memcpy(tmp + 1, pBuffer, number_bytes_to_write);

	ret = _interface.write(_address, (const char *)tmp, number_bytes_to_write + 1, false);

	if (ret) return 1;
	return 0;
}



// Return 0 in case of success
int32_t LKCoreIMU::get_ID()
{
	lsm6dsox_id_t val;
	if (lsm6dsox_id_get(&_register_io_function, NULL, &val) != 0)
		return 1;
	else
		return 0;
}

// Sets the ODR of the accelerometer and the gyroscope
// changes the value of the register CTRL1_XL and CTRL2_G
int32_t LKCoreIMU::data_rate_set(lsm6dsox_odr_xl_t odr_xl, lsm6dsox_odr_g_t odr_gy)
{
	int32_t ret;
	ret = lsm6dsox_xl_data_rate_set(&_register_io_function, odr_xl);
	if(ret == 0)
		lsm6dsox_gy_data_rate_set(&_register_io_function, odr_gy);
	return ret;
}


int32_t LKCoreIMU::data_rate_get(lsm6dsox_odr_xl_t *odr_xl, lsm6dsox_odr_g_t *odr_gy)
{
	int32_t ret;
	ret = lsm6dsox_xl_data_rate_get(&_register_io_function, odr_xl);
	if(ret == 0)
		lsm6dsox_gy_data_rate_get(&_register_io_function, odr_gy);
	return ret;
}

void LKCoreIMU::TurnOffEmbeddedFeatures(lsm6dsox_emb_sens_t *emb_sens)
{
	lsm6dsox_embedded_sens_get(&_register_io_function, emb_sens);
	lsm6dsox_embedded_sens_off(&_register_io_function);
}

void LKCoreIMU::TurnOffSensors()
{
	lsm6dsox_xl_data_rate_set(&_register_io_function, LSM6DSOX_XL_ODR_OFF);
	lsm6dsox_gy_data_rate_set(&_register_io_function, LSM6DSOX_GY_ODR_OFF);
}

void LKCoreIMU::SetFullScale()
{
	lsm6dsox_xl_full_scale_set(&_register_io_function, LSM6DSOX_4g);
	lsm6dsox_gy_full_scale_set(&_register_io_function, LSM6DSOX_2000dps);
}

void LKCoreIMU::RouteSignalsInterruptGetSet(lsm6dsox_pin_int1_route_t *pin_int1_route)
{
	lsm6dsox_pin_int1_route_get(&_register_io_function, pin_int1_route);
	pin_int1_route->mlc1 = PROPERTY_ENABLE;
	lsm6dsox_pin_int1_route_set(&_register_io_function, *pin_int1_route);
}

// Sets the ODR of the MLC
int32_t LKCoreIMU::mlc_data_rate_set(lsm6dsox_mlc_odr_t val)
{
	int32_t ret;
	ret = lsm6dsox_mlc_data_rate_set(&_register_io_function, val);
	return ret;
}

//stored in val_
int32_t LKCoreIMU::mlc_data_rate_get(lsm6dsox_mlc_odr_t *val)
{
	int32_t ret;
	ret = lsm6dsox_mlc_data_rate_get(&_register_io_function, val);
	return ret;
}


int32_t LKCoreIMU::block_data_update_set(uint8_t val){
	int32_t ret;
	ret = lsm6dsox_block_data_update_set(&_register_io_function, val);
	return ret;
}

int32_t LKCoreIMU::reset_set(uint8_t val){
	//lsm6dsox_reset_set(&_register_io_function, val);
	int32_t ret;
	ret = lsm6dsox_reset_set(&_register_io_function, val);

	return ret;
}

int32_t LKCoreIMU::reset_get(uint8_t *val){
	int32_t ret;
	ret = lsm6dsox_reset_get(&_register_io_function, val);
	return ret;
}

int32_t LKCoreIMU::int_notification_set(lsm6dsox_lir_t val){
	int32_t ret;
	ret = lsm6dsox_int_notification_set(&_register_io_function, val);
	return ret;
}

int32_t LKCoreIMU::embedded_sens_set(lsm6dsox_emb_sens_t *val){
	int32_t ret;
	ret = lsm6dsox_embedded_sens_set(&_register_io_function, val);
	return ret;
}

int32_t LKCoreIMU::rounding_mode_set(lsm6dsox_rounding_t val){
	int32_t ret;
	ret = lsm6dsox_rounding_mode_set(&_register_io_function, val);
	return ret;
}

int32_t LKCoreIMU::all_sources_get(lsm6dsox_all_sources_t *val){
	int32_t ret;
	ret = lsm6dsox_all_sources_get(&_register_io_function, val);
	return ret;
}

int32_t LKCoreIMU::mlc_out_get(uint8_t *buff){
	int32_t ret;
	ret = lsm6dsox_mlc_out_get(&_register_io_function, buff);
	return ret;
}


}	// namespace leka

void runIMU(leka::LKCoreIMU *imu)
{
	std::array<float, 3> xl_data;
	std::array<float, 3> gy_data;

	imu->getData(xl_data, gy_data);
	printf("Accelerometer data (in mg): %d %d %d\n", (int)xl_data[0], (int)xl_data[1], (int)xl_data[2]);
	printf("Gyroscope data (in mdps): %d %d %d\n\n", (int)gy_data[0], (int)gy_data[1], (int)gy_data[2]);
}

/*if(coreimu.int_notification_set(LSM6DSOX_BASE_PULSED_EMB_LATCHED)==0)
	log_debug("int_notification_set() CHECKED ");
else
	log_debug("int_notification_set() NOT CHECKED ");*/
