# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(StateMachineKit STATIC)

target_include_directories(StateMachineKit
	PUBLIC
		include
)

target_sources(StateMachineKit
	PRIVATE
		source/StateMachine.cpp
)

target_link_libraries(StateMachineKit
	PRIVATE

)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/StateMachine_test.cpp
	)

endif()
