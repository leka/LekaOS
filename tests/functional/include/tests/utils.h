// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/ThisThread.h"

#include "storage/blockdevice/COMPONENT_SD/include/SD/SDBlockDevice.h"
#include "storage/filesystem/fat/include/fat/FATFileSystem.h"

#include "LogKit.h"

namespace tests::utils {

namespace time {

	using namespace leka;
	using namespace std::chrono;

	namespace main {

		inline auto start = rtos::Kernel::Clock::now();
		inline auto stop  = rtos::Kernel::Clock::now();
		inline auto delta = [] { return static_cast<int>((stop - start).count()); };

	}	// namespace main

	namespace test {

		inline auto start = rtos::Kernel::Clock::now();
		inline auto stop  = rtos::Kernel::Clock::now();
		inline auto delta = [] { return static_cast<int>((stop - start).count()); };

	}	// namespace test

}	// namespace time

namespace sd {

	namespace internal {

		inline auto bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
		inline auto fs = FATFileSystem {"fs"};

		constexpr auto default_frequency = uint64_t {25'000'000};

	}	// namespace internal

	inline void init()
	{
		internal::bd.init();
		internal::bd.frequency(internal::default_frequency);
		internal::fs.mount(&internal::bd);
	}

}	// namespace sd

}	// namespace tests::utils
