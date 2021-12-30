# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(mbed-os)

add_library(mbed-os-static INTERFACE)

target_include_directories(mbed-os-static
	INTERFACE
		${HEADERS_DIR}/mbed

		${EXTERN_MBED_OS_DIR}

		${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/doubles
		${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/doubles/equeue

		${EXTERN_MBED_OS_DIR}/cmsis/tests/UNITTESTS/doubles

		${EXTERN_MBED_OS_DIR}/connectivity/tests/UNITTESTS/doubles
		${EXTERN_MBED_OS_DIR}/connectivity/tests/UNITTESTS/doubles/connectivity

		${EXTERN_MBED_OS_DIR}/drivers/tests/UNITTESTS/doubles
		${EXTERN_MBED_OS_DIR}/drivers/tests/UNITTESTS/doubles/drivers

		${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/doubles
		${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/doubles/events

		${EXTERN_MBED_OS_DIR}/hal/tests/UNITTESTS/doubles
		${EXTERN_MBED_OS_DIR}/hal/tests/UNITTESTS/doubles/hal

		${EXTERN_MBED_OS_DIR}/platform/tests/UNITTESTS/doubles
		${EXTERN_MBED_OS_DIR}/platform/tests/UNITTESTS/doubles/platform

		${EXTERN_MBED_OS_DIR}/rtos/tests/UNITTESTS/doubles
		${EXTERN_MBED_OS_DIR}/rtos/tests/UNITTESTS/doubles/rtos

		${EXTERN_MBED_OS_DIR}/storage/tests/UNITTESTS/doubles
		${EXTERN_MBED_OS_DIR}/storage/tests/UNITTESTS/doubles/storage

		${EXTERN_MBED_OS_DIR}/connectivity
		${EXTERN_MBED_OS_DIR}/connectivity/include
		${EXTERN_MBED_OS_DIR}/connectivity/include/connectivity

		${EXTERN_MBED_OS_DIR}/drivers
		${EXTERN_MBED_OS_DIR}/drivers/include
		${EXTERN_MBED_OS_DIR}/drivers/include/drivers

		${EXTERN_MBED_OS_DIR}/events
		${EXTERN_MBED_OS_DIR}/events/include
		${EXTERN_MBED_OS_DIR}/events/include/events

		${EXTERN_MBED_OS_DIR}/hal
		${EXTERN_MBED_OS_DIR}/hal/include
		${EXTERN_MBED_OS_DIR}/hal/include/hal

		${EXTERN_MBED_OS_DIR}/platform
		${EXTERN_MBED_OS_DIR}/platform/include
		${EXTERN_MBED_OS_DIR}/platform/include/platform

		${EXTERN_MBED_OS_DIR}/rtos
		${EXTERN_MBED_OS_DIR}/rtos/include
		${EXTERN_MBED_OS_DIR}/rtos/include/rtos
		${EXTERN_MBED_OS_DIR}/rtos/include/rtos/internal

		${EXTERN_MBED_OS_DIR}/storage
		${EXTERN_MBED_OS_DIR}/storage/filesystem/fat/ChaN
		${EXTERN_MBED_OS_DIR}/storage/include
		${EXTERN_MBED_OS_DIR}/storage/include/storage

		${EXTERN_MBED_OS_DIR}/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW
		${EXTERN_MBED_OS_DIR}/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/CMSIS
		${EXTERN_MBED_OS_DIR}/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/STM32F7xx_HAL_Driver
		${EXTERN_MBED_OS_DIR}/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/STM32F7xx_HAL_Driver/Legacy
)

target_compile_options(mbed-os-static
	INTERFACE
		-include${HEADERS_DIR}/mbed/mbed_config.h
		-include${HEADERS_DIR}/mbed/mbed_target_config.h
		-include${HEADERS_DIR}/mbed/PinNames.h
		-include${HEADERS_DIR}/mbed/PinNamesTypes.h
)
