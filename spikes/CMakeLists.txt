# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_custom_target(spikes)

function(register_spike)
	set(options "")
	set(oneValueArgs TARGET)
	set(multiValueArgs INCLUDE_DIRECTORIES SOURCES LINK_LIBRARIES)

	cmake_parse_arguments(REGISTER_SPIKE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set(REGISTER_SPIKE_TARGET_FULL_NAME spike_${REGISTER_SPIKE_TARGET})

	add_executable(${REGISTER_SPIKE_TARGET_FULL_NAME})

	target_include_directories(${REGISTER_SPIKE_TARGET_FULL_NAME}
		PRIVATE
			${REGISTER_SPIKE_INCLUDE_DIRECTORIES}
	)

	target_sources(${REGISTER_SPIKE_TARGET_FULL_NAME}
		PRIVATE
			${REGISTER_SPIKE_SOURCES}
	)

	target_link_libraries(${REGISTER_SPIKE_TARGET_FULL_NAME}
		PRIVATE
			${REGISTER_SPIKE_LINK_LIBRARIES}
	)

	target_compile_definitions(${REGISTER_SPIKE_TARGET_FULL_NAME}
		PUBLIC
			TARGET="${REGISTER_SPIKE_TARGET_FULL_NAME}"
	)

	mbed_set_post_build(${REGISTER_SPIKE_TARGET_FULL_NAME})

	add_dependencies(spikes
		${REGISTER_SPIKE_TARGET_FULL_NAME}
	)
endfunction(register_spike)

add_subdirectory(${SPIKES_DIR}/lk_behavior_kit)
add_subdirectory(${SPIKES_DIR}/lk_ble)
add_subdirectory(${SPIKES_DIR}/lk_bluetooth)
add_subdirectory(${SPIKES_DIR}/lk_cg_animations)
add_subdirectory(${SPIKES_DIR}/lk_color_kit)
add_subdirectory(${SPIKES_DIR}/lk_command_kit)
add_subdirectory(${SPIKES_DIR}/lk_config_kit)
add_subdirectory(${SPIKES_DIR}/lk_coreled)
add_subdirectory(${SPIKES_DIR}/lk_core_touch_sensor)
add_subdirectory(${SPIKES_DIR}/lk_event_queue)
add_subdirectory(${SPIKES_DIR}/lk_file_reception)
add_subdirectory(${SPIKES_DIR}/lk_file_manager_kit)
add_subdirectory(${SPIKES_DIR}/lk_flash_memory)
add_subdirectory(${SPIKES_DIR}/lk_fs)
add_subdirectory(${SPIKES_DIR}/lk_imu_kit)
add_subdirectory(${SPIKES_DIR}/lk_lcd)
add_subdirectory(${SPIKES_DIR}/lk_led_kit)
add_subdirectory(${SPIKES_DIR}/lk_log_kit)
add_subdirectory(${SPIKES_DIR}/lk_motion_kit)
add_subdirectory(${SPIKES_DIR}/lk_motors)
add_subdirectory(${SPIKES_DIR}/lk_qdac)
add_subdirectory(${SPIKES_DIR}/lk_reinforcer)
add_subdirectory(${SPIKES_DIR}/lk_rfid)
add_subdirectory(${SPIKES_DIR}/lk_sensors_battery)
add_subdirectory(${SPIKES_DIR}/lk_sensors_imu_lsm6dsox)
add_subdirectory(${SPIKES_DIR}/lk_sensors_imu_lsm6dsox_fusion_calibration)
add_subdirectory(${SPIKES_DIR}/lk_sensors_light)
add_subdirectory(${SPIKES_DIR}/lk_sensors_microphone)
add_subdirectory(${SPIKES_DIR}/lk_sensors_temperature_humidity)
add_subdirectory(${SPIKES_DIR}/lk_serial_number)
add_subdirectory(${SPIKES_DIR}/lk_ticker_timeout)
add_subdirectory(${SPIKES_DIR}/lk_touch_sensor_kit)
add_subdirectory(${SPIKES_DIR}/lk_watchdog_isr)
add_subdirectory(${SPIKES_DIR}/lk_wifi)

add_subdirectory(${SPIKES_DIR}/lk_update_process_app_base)
add_subdirectory(${SPIKES_DIR}/lk_update_process_app_update)

add_subdirectory(${SPIKES_DIR}/mbed_blinky)
add_subdirectory(${SPIKES_DIR}/mbed_watchdog_ticker_vs_thread)

add_subdirectory(${SPIKES_DIR}/stl_cxxsupport)
