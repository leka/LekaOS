# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

target_sources(LekaOSUnitTests
	PRIVATE
		Example1.test.cpp
		Example2.test.cpp
		Example3.test.cpp
		${MBED_OS_DIR}/UNITTESTS/stubs/ThisThread_stub.cpp
)
