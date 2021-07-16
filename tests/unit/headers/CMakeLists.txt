# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(mbed-os)

add_library(mbed-os INTERFACE)

target_include_directories(mbed-os
	INTERFACE
		${HEADERS_DIR}/mbed

		${EXTERN_MBED_OS_DIR}/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW
		${EXTERN_MBED_OS_DIR}/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/CMSIS
		${EXTERN_MBED_OS_DIR}/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/STM32F7xx_HAL_Driver
		${EXTERN_MBED_OS_DIR}/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/STM32F7xx_HAL_Driver/Legacy

		${EXTERN_MBED_OS_DIR}/UNITTESTS/target_h
		${EXTERN_MBED_OS_DIR}/UNITTESTS/target_h/sys
		${EXTERN_MBED_OS_DIR}/UNITTESTS/target_h/rtos
		${EXTERN_MBED_OS_DIR}/UNITTESTS/target_h/drivers
		${EXTERN_MBED_OS_DIR}/UNITTESTS/target_h/platform
		${EXTERN_MBED_OS_DIR}/UNITTESTS/target_h/platform/cxxsupport

		${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/target_h
		${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/target_h/equeue

		${EXTERN_MBED_OS_DIR}
		${EXTERN_MBED_OS_DIR}/features
		${EXTERN_MBED_OS_DIR}/platform/include
		${EXTERN_MBED_OS_DIR}/platform/include/platform
		${EXTERN_MBED_OS_DIR}/platform/mbed-trace/include
		${EXTERN_MBED_OS_DIR}/storage/filesystem/littlefs/include
		${EXTERN_MBED_OS_DIR}/storage/filesystem/fat/include
		${EXTERN_MBED_OS_DIR}/storage/filesystem/fat/ChaN
		${EXTERN_MBED_OS_DIR}/storage/blockdevice/include
		${EXTERN_MBED_OS_DIR}/storage/filesystem/include
		${EXTERN_MBED_OS_DIR}/storage/kvstore/include
		${EXTERN_MBED_OS_DIR}/storage/kvstore/kv_config
		${EXTERN_MBED_OS_DIR}/storage/kvstore/kv_config/include
		${EXTERN_MBED_OS_DIR}/drivers
		${EXTERN_MBED_OS_DIR}/drivers/include
		${EXTERN_MBED_OS_DIR}/drivers/include/drivers
		${EXTERN_MBED_OS_DIR}/drivers/include/drivers/internal
		${EXTERN_MBED_OS_DIR}/hal
		${EXTERN_MBED_OS_DIR}/hal/include
		${EXTERN_MBED_OS_DIR}/hal/include/hal
		${EXTERN_MBED_OS_DIR}/events/include
		${EXTERN_MBED_OS_DIR}/events/include/events/internal
		${EXTERN_MBED_OS_DIR}/events/source
		${EXTERN_MBED_OS_DIR}/rtos/include
		${EXTERN_MBED_OS_DIR}/features/frameworks
		${EXTERN_MBED_OS_DIR}/connectivity/libraries/nanostack-libservice
		${EXTERN_MBED_OS_DIR}/connectivity/libraries/nanostack-libservice/mbed-client-libservice
		${EXTERN_MBED_OS_DIR}/connectivity/netsocket/include
		${EXTERN_MBED_OS_DIR}/features/filesystem/fat
		${EXTERN_MBED_OS_DIR}/features/filesystem/fat/ChaN
		${EXTERN_MBED_OS_DIR}/features/filesystem/bd
		${EXTERN_MBED_OS_DIR}/features/filesystem/
		${EXTERN_MBED_OS_DIR}/features/filesystem/littlefs
		${EXTERN_MBED_OS_DIR}/features/filesystem/littlefs/littlefs
		${EXTERN_MBED_OS_DIR}/connectivity/cellular/include/cellular/framework/API
		${EXTERN_MBED_OS_DIR}/connectivity/cellular/include/cellular/framework/AT
		${EXTERN_MBED_OS_DIR}/connectivity/cellular/include/cellular/framework/device
		${EXTERN_MBED_OS_DIR}/connectivity/cellular/include/cellular/framework
		${EXTERN_MBED_OS_DIR}/connectivity/cellular/include/cellular/framework/common
		${EXTERN_MBED_OS_DIR}/connectivity
		${EXTERN_MBED_OS_DIR}/connectivity/lorawan/include/lorawan
		${EXTERN_MBED_OS_DIR}/connectivity/lorawan/lorastack
		${EXTERN_MBED_OS_DIR}/connectivity/lorawan/lorastack/mac
		${EXTERN_MBED_OS_DIR}/connectivity/lorawan/lorastack/phy
		${EXTERN_MBED_OS_DIR}/connectivity/lorawan
		${EXTERN_MBED_OS_DIR}/connectivity/mbedtls
		${EXTERN_MBED_OS_DIR}/connectivity/mbedtls/include
)

target_compile_options(mbed-os
	INTERFACE
		-include${HEADERS_DIR}/mbed/mbed_config.h
		-include${HEADERS_DIR}/mbed/mbed_target_config.h
		-include${HEADERS_DIR}/mbed/PinNames.h
		-include${HEADERS_DIR}/mbed/PinNamesTypes.h
)
