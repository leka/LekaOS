# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

project(mbed-os)

add_library(mbed-os INTERFACE)

target_include_directories(mbed-os
	INTERFACE
		${UNITTESTS_DIR}/mbed-os
		${UNITTESTS_DIR}/mbed-os/stubs
		${UNITTESTS_DIR}/mbed-os/include
		${UNITTESTS_DIR}/mbed-os/mocks

		${ROOT_DIR}/drivers/LKCoreSTM32Hal/include
		${ROOT_DIR}/extern/mbed-os/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW
		${ROOT_DIR}/extern/mbed-os/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/CMSIS
		${ROOT_DIR}/extern/mbed-os/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/STM32F7xx_HAL_Driver
		${ROOT_DIR}/extern/mbed-os/targets/TARGET_STM/TARGET_STM32F7/STM32Cube_FW/STM32F7xx_HAL_Driver/Legacy

		${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h
		${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h/platform
		${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h/platform/cxxsupport
		${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h/drivers
		${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h/rtos/include

		${ROOT_DIR}/extern/mbed-os/UNITTESTS/stubs

		${ROOT_DIR}/extern/mbed-os/events/tests/UNITTESTS/target_h
		${ROOT_DIR}/extern/mbed-os/events/tests/UNITTESTS/target_h/equeue

		${ROOT_DIR}/extern/mbed-os
		${ROOT_DIR}/extern/mbed-os/features
		${ROOT_DIR}/extern/mbed-os/platform/include
		${ROOT_DIR}/extern/mbed-os/platform/include/platform
		${ROOT_DIR}/extern/mbed-os/platform/mbed-trace/include
		${ROOT_DIR}/extern/mbed-os/storage/filesystem/littlefs/include
		${ROOT_DIR}/extern/mbed-os/storage/filesystem/fat/include
		${ROOT_DIR}/extern/mbed-os/storage/blockdevice/include
		${ROOT_DIR}/extern/mbed-os/storage/filesystem/include
		${ROOT_DIR}/extern/mbed-os/storage/kvstore/include
		${ROOT_DIR}/extern/mbed-os/storage/kvstore/kv_config
		${ROOT_DIR}/extern/mbed-os/storage/kvstore/kv_config/include
		${ROOT_DIR}/extern/mbed-os/drivers
		${ROOT_DIR}/extern/mbed-os/drivers/include
		${ROOT_DIR}/extern/mbed-os/drivers/include/drivers
		${ROOT_DIR}/extern/mbed-os/drivers/include/drivers/internal
		${ROOT_DIR}/extern/mbed-os/hal
		${ROOT_DIR}/extern/mbed-os/hal/include
		${ROOT_DIR}/extern/mbed-os/hal/include/hal
		${ROOT_DIR}/extern/mbed-os/events/include
		${ROOT_DIR}/extern/mbed-os/events/include/events/internal
		${ROOT_DIR}/extern/mbed-os/events/source
		${ROOT_DIR}/extern/mbed-os/rtos/include
		${ROOT_DIR}/extern/mbed-os/features/frameworks
		${ROOT_DIR}/extern/mbed-os/connectivity/libraries/nanostack-libservice
		${ROOT_DIR}/extern/mbed-os/connectivity/libraries/nanostack-libservice/mbed-client-libservice
		${ROOT_DIR}/extern/mbed-os/connectivity/netsocket/include
		${ROOT_DIR}/extern/mbed-os/features/filesystem/fat
		${ROOT_DIR}/extern/mbed-os/features/filesystem/fat/ChaN
		${ROOT_DIR}/extern/mbed-os/features/filesystem/bd
		${ROOT_DIR}/extern/mbed-os/features/filesystem/
		${ROOT_DIR}/extern/mbed-os/features/filesystem/littlefs
		${ROOT_DIR}/extern/mbed-os/features/filesystem/littlefs/littlefs
		${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework/API
		${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework/AT
		${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework/device
		${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework
		${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework/common
		${ROOT_DIR}/extern/mbed-os/connectivity
		${ROOT_DIR}/extern/mbed-os/connectivity/lorawan/include/lorawan
		${ROOT_DIR}/extern/mbed-os/connectivity/lorawan/lorastack
		${ROOT_DIR}/extern/mbed-os/connectivity/lorawan/lorastack/mac
		${ROOT_DIR}/extern/mbed-os/connectivity/lorawan/lorastack/phy
		${ROOT_DIR}/extern/mbed-os/connectivity/lorawan
		${ROOT_DIR}/extern/mbed-os/connectivity/mbedtls
		${ROOT_DIR}/extern/mbed-os/connectivity/mbedtls/include
)

target_sources(mbed-os
	INTERFACE
		${MBED_STUBS_DIR}/stub_ThisThread.cpp
		${MBED_STUBS_DIR}/stub_PwmOut.cpp
		${MBED_STUBS_DIR}/stub_gpio_api.cpp
)

target_compile_options(mbed-os
	INTERFACE
		-include${MBED_INCLUDE_DIR}/mbed_config.h
		-include${MBED_INCLUDE_DIR}/mbed_target_config.h
		-include${MBED_INCLUDE_DIR}/PinNames.h
		-include${MBED_INCLUDE_DIR}/PinNamesTypes.h
)
