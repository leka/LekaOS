# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

# C++ wrappers & low level drivers
add_subdirectory(${DRIVERS_DIR}/CoreI2C)
add_subdirectory(${DRIVERS_DIR}/CorePwm)
add_subdirectory(${DRIVERS_DIR}/LKCoreLL)
add_subdirectory(${DRIVERS_DIR}/CoreFs)
add_subdirectory(${DRIVERS_DIR}/LKCoreSTM32Hal)
add_subdirectory(${DRIVERS_DIR}/CoreBufferedSerial)

# LCD drivers
add_subdirectory(${DRIVERS_DIR}/LKCoreVideo)

# Connectivity drivers
add_subdirectory(${DRIVERS_DIR}/CoreWifi)

# Sensors drivers
add_subdirectory(${DRIVERS_DIR}/CoreHTS)
add_subdirectory(${DRIVERS_DIR}/CoreBattery)
add_subdirectory(${DRIVERS_DIR}/LKCoreMicrophone)
add_subdirectory(${DRIVERS_DIR}/LKCoreLightSensor)

# Actuators drivers
add_subdirectory(${DRIVERS_DIR}/FastLED)
add_subdirectory(${DRIVERS_DIR}/CoreMotor)
add_subdirectory(${DRIVERS_DIR}/CoreAudio)
