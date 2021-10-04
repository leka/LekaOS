// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SERIAL_KIT_H_
#define _LEKA_OS_SERIAL_KIT_H_

#include "drivers/BufferedSerial.h"
#include "drivers/Timer.h"
#include "events/EventQueue.h"
#include "rtos/EventFlags.h"
#include "rtos/Thread.h"

// #include "FileReception.h"
#include "FileSystemKit.h"

namespace leka {

#define SERIALKIT_NEW_MESSAGE	   (1UL << 0)
#define SERIALKIT_DOWNLOAD_STARTED (1UL << 1)
#define SERIALKIT_DOWNLOAD_ENDED   (1UL << 2)
#define SERIALKIT_DOWNLOAD_FAIL	   (1UL << 7)
#define SERIALKIT_STOP			   (1UL << 7)

#define SERIALKIT_NOTIFICATION (256UL << 0)

enum class FileReceptionOperation
{
	none,
	file_name,
	file_size,
	file_reception
};

using file_reception_operation_t = FileReceptionOperation;

class SerialKit
{
  public:
	SerialKit(mbed::BufferedSerial &serial, rtos::EventFlags &event_flags);

	void startFileReception();
	void stopFileReception();

	auto newDataIsAvailable() -> bool;

	void setBufferSize(size_t size);

	void testWrite();

	void setFilePath(std::string full_path);
	void setExpectedFileSize(size_t size);
	auto isReadyToReceiveFile() -> bool;

	auto analyzer() -> file_reception_operation_t;
	void dispatcher(file_reception_operation_t operation, lstd::span<uint8_t> data);
	void receiveFile();

	void receiveData();
	void calibrate();

  private:
	mbed::BufferedSerial &_serial;
	FileSystemKit::File file;

	rtos::Thread _thread;
	events::EventQueue _event_queue;
	int _event_id;
	rtos::EventFlags &_event_flags;
	std::vector<uint8_t> _reception_buffer;
	std::chrono::seconds _timeout;
	mbed::Timer _timer;

	std::string _path;
	size_t reception_bytes {0};
	size_t _expected_file_size;
	long long total_duration;
};

}	// namespace leka

#endif	 // _LEKA_OS_SERIAL_KIT_H_
