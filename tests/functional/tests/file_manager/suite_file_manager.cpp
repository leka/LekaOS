// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <filesystem>

#include "FileManagerKit.h"
#include "boost/ut.hpp"
#include "tests/config.h"
#include "tests/utils.h"

using namespace leka;
using namespace boost::ut;

struct path {
	static inline const auto root = std::filesystem::path {"/fs/var/tmp/test"};

	static inline const auto dir		  = root / "dir" / "";
	static inline const auto sub_dir	  = root / "dir" / "sub";
	static inline const auto dir_file	  = root / "dir" / "dir_file.txt";
	static inline const auto sub_dir_file = root / "dir" / "sub" / "sub_dir_file.txt";

	static inline const auto all = std::to_array({dir_file, sub_dir, dir, root});

	static inline auto remove_all = []() {
		log << "";
		log << "Cleaning up files, directories";

		for (const auto &path: path::all) {
			if (std::filesystem::exists(path)) {
				log << "Removing:" << path;
				std::filesystem::remove(path);
				expect(not std::filesystem::exists(path)) << path << "still exists";
			} else {
				log << "Doesn't exit:" << path;
			}
		}
		log << "";
	};
};

suite suite_file_manager_kit = [] {
	auto file		= FileManagerKit::File {};
	auto input_data = std::to_array("Hello, Leka!");

	"initialize SD card"_test = [] {
		tests::utils::sd::init();
		expect(tests::utils::sd::internal::bd.size() != 0_u);
	};

	"remove existing files"_test = [] { path::remove_all(); };

	"open existing file"_test = [&] {
		auto open = file.open("/fs/usr/test/file-1-ok.txt", "w");

		expect(open >> fatal) << "Failed to open file";
	};

	"create root directory"_test = [&] {
		auto created = FileManagerKit::create_directory(path::root);

		expect(created >> fatal) << "Failed to create directory" << path::root;
	};

	"create directory"_test = [&] {
		auto created = FileManagerKit::create_directory(path::dir);

		expect(created >> fatal) << "Failed to create directory" << path::dir;
	};

	"create subdirectory"_test = [&] {
		auto created = FileManagerKit::create_directory(path::sub_dir);

		expect(created >> fatal) << "Failed to create directory";
	};

	"create & open new file"_test = [&] {
		auto open	= file.open(path::dir_file, "w");
		auto exists = std::filesystem::exists(path::dir_file);

		expect(open >> fatal) << "Failed to open file";
		expect(exists >> fatal) << "Failed to create file";
	};

	"close new file"_test = [&] {
		file.close();

		expect(not file.is_open()) << "Failed to close file";
	};

	"new file exists"_test = [&] {
		auto exists = FileManagerKit::file_exists(path::dir_file);

		expect(exists >> fatal) << "Failed to check the existance of file";
	};

	"write to new file"_test = [&] {
		file.open(path::dir_file, "w");
		auto bytes = file.write(input_data);
		file.close();

		expect(bytes == std::size(input_data) >> fatal) << "Failed to write file";
	};

	"read new file"_test = [&] {
		auto output_data = std::array<char, 64> {};

		file.open(path::dir_file, "r");
		auto bytes = file.read(output_data);
		file.close();

		expect(bytes == std::size(input_data) >> fatal) << "Failed to read file, bytes =" << bytes;

		auto found = std::ranges::search(output_data, input_data);

		expect(not found.empty());
	};

	"close file"_test = [&] {
		file.close();
		expect(not file.is_open());
	};

	"clean up created files"_test = [] { path::remove_all(); };

	"file does not exist"_test = [&] {
		auto dir_file_is_missing = FileManagerKit::file_is_missing(path::dir_file);

		expect(dir_file_is_missing >> fatal) << "Failed to check the non existance of file";
	};
};
