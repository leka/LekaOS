# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(TESTS_FUNCTIONAL_INCLUDE_DIR ${TESTS_FUNCTIONAL_DIR}/include)
set(TESTS_FUNCTIONAL_SOURCE_DIR ${TESTS_FUNCTIONAL_DIR}/source)
set(TESTS_FUNCTIONAL_TESTS_DIR ${TESTS_FUNCTIONAL_DIR}/tests)

include(register_functional_test)

add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/boost_ut)

add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/core_imu)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/deep_sleep_core_buffered_serial)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/deep_sleep_core_lcd)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/deep_sleep_core_motor)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/deep_sleep_core_pwm)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/deep_sleep_imu)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/deep_sleep_log_kit)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/deep_sleep_mbed_hal)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/deep_sleep_rfid_kit)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/file_manager)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/firmware_kit)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/imu_kit)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/io_expander)
add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/qdac)

add_custom_target(tests_functional)
add_dependencies(tests_functional
	functional_ut_boost_ut
	functional_ut_core_imu
	functional_ut_deep_sleep_core_buffered_serial
	functional_ut_deep_sleep_core_motor
	functional_ut_deep_sleep_core_pwm
	functional_ut_deep_sleep_log_kit
	functional_ut_deep_sleep_mbed_hal
	functional_ut_file_manager
	functional_ut_firmware_kit
	functional_ut_imu_kit
	functional_ut_io_expander
	functional_ut_qdac
)
