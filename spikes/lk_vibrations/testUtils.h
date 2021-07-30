
// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_TEST_UTILS_H_
#define _LEKA_OS_SPIKE_TEST_UTILS_H_

#include "mbed.h"
#include <array>
#include <chrono>
#include <cstdint>

using namespace std::chrono;

void testFloatWorkflow(std::array<uint16_t, 16> &outBuffer)
{
	// float sinBuffer[16];
	std::array<float, 16> sinBuffer {};
	// printf("Float#############\n");

	// random values
	float vibAmpli = 0.1;
	float sinAmpli = 0.4 * 0xFFF;
	float env	   = 0.3;
	float offset   = 0.45 * 0xFFF;

	// first calculations of sinus
	// ampli is calculated here
	for (uint32_t i = 0; i < 16; ++i) {
		sinBuffer.at(i) = static_cast<float>(vibAmpli * sinAmpli * sin(i * 2.0 * M_PI / 16));
		//	printf("%.3f\n", sinBuffer[i]);
	}

	// consider 1000 periods

	for (uint32_t i = 0; i < 10000; ++i) {
		for (uint32_t j = 0; j < 16; ++j) {
			outBuffer.at(j) = static_cast<uint16_t>(static_cast<float>(env) * sinBuffer.at(j) + offset);
		}
	}
}

void testIntWorkflow(std::array<uint16_t, 16> &outBuffer)
{
	// int16_t sinBuffer[16];
	std::array<int16_t, 16> sinBuffer {};
	// printf("Int#############\n");

	// random values
	float vibAmpli	  = 0.1;
	uint16_t sinAmpli = 0.4 * 0xFFF;
	float env		  = 0.3;
	uint16_t offset	  = 0.45 * 0xFFF;

	// first calculations of sinus
	// ampli is calculated here
	for (uint32_t i = 0; i < 16; ++i) {
		sinBuffer.at(i) = static_cast<int16_t>(vibAmpli * static_cast<float>(sinAmpli) * sin(i * 2.0 * M_PI / 16));
		// printf("%d\n", sinBuffer[i]);
	}

	// consider 1000 periods
	for (uint32_t i = 0; i < 10000; ++i) {
		for (uint32_t j = 0; j < 16; ++j) {
			outBuffer.at(j) =
				static_cast<uint16_t>(static_cast<float>(sinBuffer.at(j)) * env + static_cast<float>(offset));
		}
	}
}

void testUintWorkflow(std::array<uint16_t, 16> &outBuffer)
{
	// uint16_t sinBuffer[16];
	std::array<uint16_t, 16> sinBuffer {};
	// printf("Uint#############\n");

	float vibAmpli = 0.1;
	float sinAmpli = 0.4;
	float env	   = 0.3;
	float offset   = 0.45;

	// first calculations of sinus]
	for (uint32_t i = 0; i < 16; ++i) {
		double tmp		= sinAmpli * vibAmpli * sin(i * 2.0 * M_PI / 16) + offset;
		sinBuffer.at(i) = static_cast<uint16_t>(tmp * UINT16_MAX);
		// printf("%lu\n", sinBuffer[i]);
	}

	// consider 1000 periods
	for (uint32_t i = 0; i < 10000; ++i) {
		for (uint32_t j = 0; j < 16; ++j) {
			outBuffer.at(j) =
				static_cast<uint16_t>(static_cast<float>(sinBuffer.at(j) - static_cast<uint16_t>(offset * UINT16_MAX)) *
										  env +
									  offset * UINT16_MAX) >>
				4;
		}
	}
}

void testWorkflows()
{
	std::array<uint16_t, 16> buff_1 {};
	std::array<uint16_t, 16> buff_2 {};
	std::array<uint16_t, 16> buff_3 {};

	Timer t;
	t.start();

	auto start = t.elapsed_time();
	testFloatWorkflow(buff_1);
	auto end = t.elapsed_time();
	printf("Float time ns: \n%lld us\n", chrono::duration_cast<chrono::microseconds>(end - start).count());

	start = t.elapsed_time();
	testIntWorkflow(buff_2);
	end = t.elapsed_time();
	printf("Int time ns: \n%lld us\n", chrono::duration_cast<chrono::microseconds>(end - start).count());

	start = t.elapsed_time();
	testUintWorkflow(buff_3);
	end = t.elapsed_time();
	printf("Uint time ns: \n%lld us\n", chrono::duration_cast<chrono::microseconds>(end - start).count());

	printf("Supposedly equal :\n");
	for (int i = 0; i < 16; ++i) {
		printf("%d\t%d\t%d\n", buff_1.at(i), buff_2.at(i), buff_3.at(i));
	}
}

#endif	 //_LEKA_OS_SPIKE_TEST_UTILS_H_