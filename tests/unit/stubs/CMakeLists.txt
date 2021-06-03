# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(UNIT_TESTS_STUBS_DIR      ${CMAKE_CURRENT_SOURCE_DIR})
set(UNIT_TESTS_STUBS_MBED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stubs/mbed)

add_library(stubs INTERFACE)

target_include_directories(stubs
	INTERFACE
		./
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs
)

target_sources(stubs
	INTERFACE
		${UNIT_TESTS_STUBS_MBED_DIR}/source/gpio_api.cpp
		${UNIT_TESTS_STUBS_MBED_DIR}/source/AnalogIn.cpp
		${UNIT_TESTS_STUBS_MBED_DIR}/source/PwmOut.cpp
		${UNIT_TESTS_STUBS_MBED_DIR}/source/I2C.cpp
		${UNIT_TESTS_STUBS_MBED_DIR}/source/Kernel.cpp
		${UNIT_TESTS_STUBS_MBED_DIR}/source/mbed_critical.cpp

		${UNIT_TESTS_STUBS_MBED_DIR}/source/EventQueue_extension.cpp

		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/equeue_stub.c
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/mbed_rtos_rtx_stub.c
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/Mutex_stub.cpp
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/rtx_mutex_stub.c
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/ThisThread_stub.cpp
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/Thread_stub.cpp
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/mbed_assert_stub.cpp
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/EventQueue_stub.cpp
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/SerialBase_stub.cpp
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/FileHandle_stub.cpp
		${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs/BufferedSerial_stub.cpp
)
