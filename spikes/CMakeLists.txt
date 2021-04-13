# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(${SPIKES_DIR}/mbed_blinky)
add_subdirectory(${SPIKES_DIR}/mbed_watchdog_ticker_vs_thread)

add_subdirectory(${SPIKES_DIR}/lk_ble)
add_subdirectory(${SPIKES_DIR}/lk_lcd)
add_subdirectory(${SPIKES_DIR}/lk_led)
add_subdirectory(${SPIKES_DIR}/lk_qspi)
add_subdirectory(${SPIKES_DIR}/lk_rfid)
add_subdirectory(${SPIKES_DIR}/lk_wifi)
add_subdirectory(${SPIKES_DIR}/lk_motors)
add_subdirectory(${SPIKES_DIR}/lk_sensor_battery)
add_subdirectory(${SPIKES_DIR}/lk_bluetooth)
add_subdirectory(${SPIKES_DIR}/lk_microphone)
add_subdirectory(${SPIKES_DIR}/lk_light_sensor)
add_subdirectory(${SPIKES_DIR}/lk_file_manager)
add_subdirectory(${SPIKES_DIR}/lk_sensors_touch)
add_subdirectory(${SPIKES_DIR}/lk_sensor_temperature)

add_custom_target(spikes_leka)
add_dependencies( spikes_leka
	spike_lk_ble
	spike_lk_lcd
	spike_lk_led
	spike_lk_qspi
	spike_lk_rfid
	spike_lk_wifi
	spike_lk_motors
	spike_lk_sensor_battery
	spike_lk_bluetooth
	spike_lk_microphone
	spike_lk_light_sensor
	spike_lk_file_manager
	spike_lk_sensors_touch
	spike_lk_sensor_temperature
)

add_custom_target(spikes_mbed)
add_dependencies( spikes_mbed
	spike_mbed_blinky
	spike_mbed_watchdog_ticker_vs_thread
)

add_custom_target(spikes)
add_dependencies(spikes
	spikes_leka
	spikes_mbed
)
