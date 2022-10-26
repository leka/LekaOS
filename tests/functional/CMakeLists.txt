# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(TESTS_FUNCTIONAL_INCLUDE_DIR ${TESTS_FUNCTIONAL_DIR}/include)
set(TESTS_FUNCTIONAL_SOURCE_DIR ${TESTS_FUNCTIONAL_DIR}/source)
set(TESTS_FUNCTIONAL_TESTS_DIR ${TESTS_FUNCTIONAL_DIR}/tests)

function(register_functional_test)
	set(options "")
	set(oneValueArgs TARGET)
	set(multiValueArgs INCLUDE_DIRECTORIES SOURCES LINK_LIBRARIES)

	cmake_parse_arguments(REGISTER_FT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	add_mbed_executable(${REGISTER_FT_TARGET})

	target_include_directories(${REGISTER_FT_TARGET}
		PRIVATE
			${TESTS_FUNCTIONAL_INCLUDE_DIR}
			${REGISTER_FT_INCLUDE_DIRECTORIES}
	)

	target_sources(${REGISTER_FT_TARGET}
		PRIVATE
			${TESTS_FUNCTIONAL_SOURCE_DIR}/test_main.cpp
			${REGISTER_FT_SOURCES}
	)

	target_link_libraries(${REGISTER_FT_TARGET}
		${REGISTER_FT_LINK_LIBRARIES}
	)

	target_compile_definitions(${REGISTER_FT_TARGET}
		PUBLIC
			TARGET="${REGISTER_FT_TARGET}"
	)
endfunction()

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

add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/stl_features)
