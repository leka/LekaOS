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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 7d8790d7 (add CoreIMU)
=======
>>>>>>> ee69a2c4 (add CoreIMU to MakeList)
add_subdirectory(${DRIVERS_DIR}/CoreQDAC)
add_subdirectory(${DRIVERS_DIR}/CoreTouchSensor)

>>>>>>> 961b0e85 (:sparkles: (drivers): Add CoreTouchSensor)

<<<<<<< HEAD
# Touch drivers
add_subdirectory(${DRIVERS_DIR}/CoreLSM6DSOX)
=======
=======

# IMU drivers
add_subdirectory(${DRIVERS_DIR}/CoreIMU)
>>>>>>> a8079684 (add CoreIMU)
<<<<<<< HEAD
>>>>>>> 7d8790d7 (add CoreIMU)
=======
=======

# Touch drivers
<<<<<<< HEAD
add_subdirectory(${DRIVERS_DIR}/CoreIMU)
>>>>>>> 81e3955d (add CoreIMU to MakeList)
<<<<<<< HEAD
>>>>>>> ee69a2c4 (add CoreIMU to MakeList)
=======
=======
add_subdirectory(${DRIVERS_DIR}/CoreLSM6DSOX)
>>>>>>> 6fac508c (:sparkles: [drivers] - Add CoreLSM6DSOX)
>>>>>>> 37cbd965 (:sparkles: [drivers] - Add CoreLSM6DSOX)
