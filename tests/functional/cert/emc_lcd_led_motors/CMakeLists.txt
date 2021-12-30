# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(certs_emc_lcd_led_motors)

target_include_directories(certs_emc_lcd_led_motors
	PRIVATE
		.
		../../include
)

target_sources(certs_emc_lcd_led_motors
	PRIVATE
		main.cpp
)

target_link_libraries(certs_emc_lcd_led_motors
	PUBLIC
		mbed-os-static
		CorePwm
		CoreMotor
		driver_FastLED
		lib_LekaScreen
)

