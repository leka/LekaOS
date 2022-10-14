// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "SDBlockDevice.h"
#include "boost/ut.hpp"
#include "config.h"
#include "filesystem"
#include "sleep.h"

using namespace leka;
using namespace boost::ut;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

suite suite_file_manager_kit = [] {
	auto file									  = FileManagerKit::File {};
	const std::filesystem::path tmp_dir			  = "/fs/tmp/";
	const std::filesystem::path test_dir		  = tmp_dir / "test/";
	const std::filesystem::path test_sub_dir	  = test_dir / "sub/";
	const std::filesystem::path test_filename	  = test_dir / "test_spike_lk_filesystem_kit";
	const std::filesystem::path test_sub_filename = test_sub_dir / "test_spike_lk_filesystem_kit";
	auto input_data								  = std::string {"Hello Leka"};

	busy_sleep(2000);
	initializeSD();
	busy_sleep(2000);

	std::filesystem::remove(test_filename);
	std::filesystem::remove(test_sub_dir);
	std::filesystem::remove(test_dir);

	auto create_test_dir	= FileManagerKit::create_directory(test_dir);
	"create directory"_test = [&] { expect(create_test_dir >> fatal) << "Failed to create directory"; };

	auto create_sub_test_dir   = FileManagerKit::create_directory(test_sub_dir);
	"create subdirectory"_test = [&] { expect(create_sub_test_dir >> fatal) << "Failed to create directory"; };

	auto open	= file.open(test_filename, "w");
	"open"_test = [&] { expect(open >> fatal) << "Failed to open file"; };

	auto bytes	 = file.write(input_data);
	"write"_test = [&] { expect(bytes == std::size(input_data) >> fatal) << "Failed to write file"; };
	file.close();
};
