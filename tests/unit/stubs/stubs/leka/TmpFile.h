// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <lstd_span>
#include <string>

namespace leka {

class TmpFile
{
  public:
	TmpFile() = default;

	void createTempFile();
	void deleteTempFile();
	auto getPath() -> std::string;

	auto readTempFile() -> std::string;
	void writeTempFile(lstd::span<uint8_t> data);
	void writeTempFile(uint8_t *data, uint32_t size);

  private:
	char tempFilename[L_tmpnam];   // NOLINT
};

}	// namespace leka
