# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(certs_emc_bt_lcd_qspi_rfid_touch_wifi)

target_include_directories(certs_emc_bt_lcd_qspi_rfid_touch_wifi
	PRIVATE
		.
		../../include
)

target_sources(certs_emc_bt_lcd_qspi_rfid_touch_wifi
	PRIVATE
		main.cpp
		CoreFlashUtils.cpp
)

target_link_libraries(certs_emc_bt_lcd_qspi_rfid_touch_wifi
	lib_LekaBluetooth
	CoreQSPI
	CoreFlashMemory
	lib_LekaRFID
	lib_LekaScreen
	lib_LekaTouch
	lib_LekaWifi
)

target_link_custom_leka_targets(certs_emc_bt_lcd_qspi_rfid_touch_wifi)
