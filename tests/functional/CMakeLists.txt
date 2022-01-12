# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(${FUNCTIONAL_TESTS_DIR}/cert/emc_ble_bt_lcd_led_motors)
add_subdirectory(${FUNCTIONAL_TESTS_DIR}/cert/emc_ble_led_motors)
add_subdirectory(${FUNCTIONAL_TESTS_DIR}/cert/emc_lcd_led_motors)
add_subdirectory(${FUNCTIONAL_TESTS_DIR}/cert/emc_bt_lcd_qspi_rfid_touch_wifi)
add_subdirectory(${FUNCTIONAL_TESTS_DIR}/hardware/motors_test_reduction_and_wheels)

add_custom_target(tests_functional)
add_dependencies(tests_functional
	certs_emc_ble_bt_lcd_led_motors
	certs_emc_ble_led_motors
	certs_emc_lcd_led_motors
	certs_emc_bt_lcd_qspi_rfid_touch_wifi
	hardware_motors_test_reduction_and_wheels
)
