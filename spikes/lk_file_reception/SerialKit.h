// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SERIAL_KIT_H_
#define _LEKA_OS_SERIAL_KIT_H_

#include "drivers/BufferedSerial.h"

#include "FileReception.h"
#include "FileSystemKit.h"

namespace leka {

class SerialKit : public interface::FileReception
{
  public:
	SerialKit(mbed::BufferedSerial &serial) : _serial(serial) {};

	void setFileName(std::string name, std::string path) final;
	void setFileSize(size_t size) final;
	auto setReadyToReceiveFile() -> bool final;

  private:
	mbed::BufferedSerial &_serial;
	FileSystemKit::File file;
};

}	// namespace leka

#endif	 // _LEKA_OS_SERIAL_KIT_H_
