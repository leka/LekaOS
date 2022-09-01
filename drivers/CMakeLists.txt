# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

# C++ wrappers & low level drivers
add_subdirectory(${DRIVERS_DIR}/CoreBufferedSerial)
add_subdirectory(${DRIVERS_DIR}/CoreEventFlags)
add_subdirectory(${DRIVERS_DIR}/CoreEventQueue)
add_subdirectory(${DRIVERS_DIR}/CoreI2C)
add_subdirectory(${DRIVERS_DIR}/CoreLL)
add_subdirectory(${DRIVERS_DIR}/CoreMCU)
add_subdirectory(${DRIVERS_DIR}/CoreMutex)
add_subdirectory(${DRIVERS_DIR}/CorePwm)
add_subdirectory(${DRIVERS_DIR}/CoreQSPI)
add_subdirectory(${DRIVERS_DIR}/CoreRFIDReader)
add_subdirectory(${DRIVERS_DIR}/CoreSPI)
add_subdirectory(${DRIVERS_DIR}/CoreSTM32Hal)
add_subdirectory(${DRIVERS_DIR}/CoreTicker)
add_subdirectory(${DRIVERS_DIR}/CoreTimeout)

# LCD drivers
add_subdirectory(${DRIVERS_DIR}/CoreVideo)

# Connectivity drivers
add_subdirectory(${DRIVERS_DIR}/CoreWifi)

# Memory drivers
add_subdirectory(${DRIVERS_DIR}/CoreFlashMemory)

# Sensors drivers
add_subdirectory(${DRIVERS_DIR}/CoreBattery)
add_subdirectory(${DRIVERS_DIR}/CoreHTS)
add_subdirectory(${DRIVERS_DIR}/CoreLightSensor)
add_subdirectory(${DRIVERS_DIR}/CoreMicrophone)

# Actuators drivers
add_subdirectory(${DRIVERS_DIR}/CoreLED)
add_subdirectory(${DRIVERS_DIR}/CoreMotor)

# Touch drivers
add_subdirectory(${DRIVERS_DIR}/CoreIOExpander)
add_subdirectory(${DRIVERS_DIR}/CoreQDAC)
add_subdirectory(${DRIVERS_DIR}/CoreTouchSensor)

# Touch drivers
add_subdirectory(${DRIVERS_DIR}/CoreLSM6DSOX)