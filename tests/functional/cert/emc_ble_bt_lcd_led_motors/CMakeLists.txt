# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(certs_emc_ble_bt_lcd_led_motors)

target_include_directories(certs_emc_ble_bt_lcd_led_motors
	PRIVATE
		.
		../../include
)

target_sources(certs_emc_ble_bt_lcd_led_motors
	PRIVATE
		main.cpp
)

target_link_libraries(certs_emc_ble_bt_lcd_led_motors
	CorePwm
	CoreMotor
	driver_FastLED
	lib_BLE
	FileManagerKit
	lib_LekaBluetooth
	lib_LekaRFID
	lib_LekaScreen
	lib_LekaTouch
)

target_link_custom_leka_targets(certs_emc_ble_bt_lcd_led_motors)
