# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(UNIT_TESTS_STUBS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(UNIT_TESTS_STUBS_LEKA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stubs/leka)
set(UNIT_TESTS_STUBS_MBED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stubs/mbed)

add_library(stubs INTERFACE)

target_include_directories(stubs
	INTERFACE
	./
	${EXTERN_MBED_OS_DIR}/UNITTESTS/stubs

	${DRIVERS_DIR}/CoreEventQueue
)

target_sources(stubs
	INTERFACE
	${UNIT_TESTS_STUBS_MBED_DIR}/source/gpio_api.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/AnalogIn.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/InterruptIn.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/PwmOut.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/QSPI.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/SPI.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/I2C.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/Timer.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/BLE.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/Kernel.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/mbed_critical.cpp
	${UNIT_TESTS_STUBS_MBED_DIR}/source/mbed_power_mgmt.cpp

	${UNIT_TESTS_STUBS_MBED_DIR}/source/EventQueue_extension.cpp

	${UNIT_TESTS_STUBS_LEKA_DIR}/source/CoreEventQueue.cpp
	${UNIT_TESTS_STUBS_LEKA_DIR}/source/EventLoopKit.cpp

	${EXTERN_MBED_OS_DIR}/rtos/tests/UNITTESTS/doubles/mbed_rtos_rtx_stub.c
	${EXTERN_MBED_OS_DIR}/rtos/tests/UNITTESTS/doubles/Mutex_stub.cpp
	${EXTERN_MBED_OS_DIR}/rtos/tests/UNITTESTS/doubles/rtx_mutex_stub.c
	${EXTERN_MBED_OS_DIR}/rtos/tests/UNITTESTS/doubles/ThisThread_stub.cpp
	${EXTERN_MBED_OS_DIR}/rtos/tests/UNITTESTS/doubles/Thread_stub.cpp
	${EXTERN_MBED_OS_DIR}/platform/tests/UNITTESTS/doubles/mbed_assert_stub.cpp
	${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/doubles/equeue_stub.c
	${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/doubles/EventFlags_stub.cpp
	${EXTERN_MBED_OS_DIR}/events/tests/UNITTESTS/doubles/EventQueue_stub.cpp
	${EXTERN_MBED_OS_DIR}/drivers/tests/UNITTESTS/doubles/SerialBase_stub.cpp
	${EXTERN_MBED_OS_DIR}/platform/tests/UNITTESTS/doubles/FileHandle_stub.cpp
	${EXTERN_MBED_OS_DIR}/drivers/tests/UNITTESTS/doubles/BufferedSerial_stub.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/source/gap/AdvertisingDataBuilder.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/source/gap/AdvertisingParameters.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/source/gap/ConnectionParameters.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/source/gatt/DiscoveredCharacteristic.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/source/Gap.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/source/GattClient.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/source/GattServer.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/source/SecurityManager.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/tests/UNITTESTS/doubles/fakes/BLE.cpp
	${EXTERN_MBED_OS_DIR}/connectivity/FEATURE_BLE/tests/UNITTESTS/doubles/fakes/source/GattServerImpl_mock.cpp
)
