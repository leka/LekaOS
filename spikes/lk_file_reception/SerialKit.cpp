#include "SerialKit.h"

#include "rtos/ThisThread.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono_literals;
using namespace std::chrono;

SerialKit::SerialKit(mbed::BufferedSerial &serial, rtos::EventFlags &event_flags)
	: _serial(serial), _event_flags(event_flags)
{
	// _thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
	_timeout	   = 5s;
	total_duration = 0;

	_path				= "";
	_expected_file_size = 0;
}

void SerialKit::setBufferSize(size_t size)
{
	_reception_buffer.resize(size);
}

void SerialKit::startFileReception()
{
	// _event_id = _event_queue.call_every(50ms, this, &SerialKit::newDataIsAvailable);

	while (true) {
		_event_flags.wait_any(SERIALKIT_NEW_MESSAGE);

		// analyzer
		// dispatch

		// if STOP, break;
	}
}

auto SerialKit::analyzer() -> file_reception_operation_t
{
	auto operation_type = FileReceptionOperation::none;
	// if (_reception_buffer.find("Name") != std::string::npos) {
	// 	operation_type = FileReceptionOperation::file_name;
	// } else if (_reception_buffer.find("Size") != std::string::npos) {
	// 	operation_type = FileReceptionOperation::file_size;
	// } else if (_reception_buffer.find("Data") != std::string::npos) {
	// 	operation_type = FileReceptionOperation::file_reception;
	// }
	return operation_type;
}

void SerialKit::dispatcher(file_reception_operation_t operation, lstd::span<uint8_t> data)
{
	switch (operation) {
		case FileReceptionOperation::file_name:
			setFilePath("b");
			break;
		case FileReceptionOperation::file_size:
			setExpectedFileSize(1);
			break;
		case FileReceptionOperation::file_reception:
			receiveFile();
			break;
		default:
			break;
	}
}

void SerialKit::stopFileReception()
{
	// _event_queue.cancel(_event_id);
	_event_flags.set(SERIALKIT_STOP);
}

void SerialKit::testWrite()
{
	file.open("fs/test_file.txt", "wx");
	file.close();

	std::vector<uint8_t> vect = {0, 1, 2, 3, 4, 5, 6, 7};
	vect.resize(128);

	_timer.reset();
	_timer.start();

	file.open("fs/test_file.txt", "a+");
	while (_serial.readable()) {
		uint32_t num = _serial.read(vect.data(), vect.size());
		file.write(vect.data(), num);
		file.close();

		rtos::ThisThread::sleep_for(1ms);
	}
	_timer.stop();
	log_debug("Operation duration: %llums", duration_cast<milliseconds>(_timer.elapsed_time()).count());
}

void SerialKit::receiveData()
{
	while (_serial.readable()) {
		uint32_t num = _serial.read(_reception_buffer.data(), _reception_buffer.size());
		file.write(_reception_buffer.data(), num);
		reception_bytes += num;

		rtos::ThisThread::sleep_for(1ms);
	}
}

void SerialKit::receiveFile()
{
	while (reception_bytes != _expected_file_size) {
		file.open(_path.data(), "a+");

		_timer.reset();
		_timer.start();

		receiveData();

		_timer.stop();

		file.close();

		auto duration = duration_cast<milliseconds>(_timer.elapsed_time()).count();
		total_duration += duration;
		if (duration != 0) {
			log_debug("Operation duration: %llums (%llums) | Bytes received %ldB", duration, total_duration,
					  reception_bytes);
		}
	}
	log_info("Download ended !");
}

auto getMinimalDelayBetweenPacket() -> std::chrono::milliseconds {}

void calibrate() {}

// void SerialKit::receiveFile()
// {
// 	_timer.reset();
// 	_event_flags.wait_any_for(SERIALKIT_NEW_MESSAGE, _timeout);
// 	if (newDataIsAvailable()) {
// 		_timer.start();

// 		file.open(_path.data(), "a+");
// 		// _event_flags.set(SERIALKIT_DOWNLOAD_STARTED);
// 		while (_timer.elapsed_time() < _timeout) {
// 			if (_serial.readable()) {
// 				// file.open(_path.data(), "a+");
// 				uint32_t data_read = _serial.read(_reception_buffer.data(), _reception_buffer.size());
// 				file.write(_reception_buffer.data(), data_read);

// 				// file.close();
// 				// _timer.reset();
// 			}
// 			rtos::ThisThread::sleep_for(1ms);
// 		}
// 		file.close();
// 		// _event_flags.set(SERIALKIT_DOWNLOAD_ENDED);
// 		// stopFileReception();

// 		_timer.stop();
// 		log_debug("Operation duration: %llums", duration_cast<milliseconds>(_timer.elapsed_time()).count());

// 	} else {
// 		_event_flags.set(SERIALKIT_DOWNLOAD_FAIL);
// 	}
// }

auto SerialKit::newDataIsAvailable() -> bool
{
	// _reception_buffer.clear();

	// while (_serial.readable()) {
	// 	std::array<uint8_t, 1> temp_value {};

	// 	_serial.read(temp_value.data(), 1);

	// 	_reception_buffer.push_back(temp_value[0]);
	// }

	// bool new_data_received = not(_reception_buffer.empty());
	// if (new_data_received) {
	// 	_event_flags.set(SERIALKIT_NEW_MESSAGE);
	// }

	// return new_data_received;

	auto new_data = _serial.readable();
	if (new_data) {
		_event_flags.set(SERIALKIT_NEW_MESSAGE);
	}

	return new_data;
}

void SerialKit::setFilePath(std::string full_path)
{
	_path = full_path;

	file.open(_path.data(), "wx");
	file.close();
}

void SerialKit::setExpectedFileSize(size_t size)
{
	_expected_file_size = size;
}

auto SerialKit::isReadyToReceiveFile() -> bool
{
	auto all_checks = !_path.empty();
	all_checks &= _expected_file_size != 0;

	log_debug("Path: %d | Size: %d", !_path.empty(), _expected_file_size != 0);

	return all_checks;
}
