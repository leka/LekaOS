// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../TmpFile.h"
#include <fstream>
#include <sstream>

using namespace leka;

void TmpFile::createTempFile()
{
	strcpy(tempFilename, "/tmp/XXXXXX");
	mkstemp(tempFilename);
}

auto TmpFile::getPath() -> std::string
{
	return std::string(tempFilename);
}

auto TmpFile::readTempFile() -> std::string
{
	std::fstream f {};
	f.open(tempFilename);

	std::stringstream out {};
	out << f.rdbuf();
	f.close();

	return out.str();
}

void TmpFile::writeTempFile(lstd::span<uint8_t> data)
{
	auto *file = fopen(tempFilename, "wb");
	fwrite(data.data(), sizeof(uint8_t), data.size(), file);
	fclose(file);
}

void TmpFile::writeTempFile(uint8_t *data, uint32_t size)
{
	auto *file = fopen(tempFilename, "wb");
	fwrite(data, sizeof(uint8_t), size, file);
	fclose(file);
}
